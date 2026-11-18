"""
Source-level regression tests for the panasonic_heatpump C++ component.

The ``set_command_*`` setters read from ``heatpump_default_message_``, which is
empty until the first response frame has been received. These tests make sure
every setter checks the vector before accessing it, so the guards cannot be
dropped accidentally.
"""

import os
import re

import pytest

CPP_PATH = os.path.join(
    os.path.dirname(__file__),
    "..",
    "..",
    "components",
    "panasonic_heatpump",
    "panasonic_heatpump.cpp",
)

SETTERS = [
    "set_command_high_nibble",
    "set_command_low_nibble",
    "set_command_byte",
    "set_command_curve",
]

GUARD_PATTERN = re.compile(
    r"this->heatpump_default_message_\.empty\(\)\s*\|\|\s*"
    r"this->heatpump_default_message_\.size\(\)\s*<=\s*index"
)


def _read_source():
    with open(CPP_PATH, encoding="utf-8") as cpp_file:
        return cpp_file.read()


def _function_body(source, setter):
    """Return the body of ``PanasonicHeatpumpComponent::<setter>``."""
    signature = f"void PanasonicHeatpumpComponent::{setter}("
    start = source.index(signature)
    body_start = source.index("{", start)

    depth = 0
    for index in range(body_start, len(source)):
        if source[index] == "{":
            depth += 1
        elif source[index] == "}":
            depth -= 1
            if depth == 0:
                body_end = index + 1
                return source[body_start:body_end]

    raise AssertionError(f"Unterminated function body for {setter}")


@pytest.mark.parametrize("setter", SETTERS)
def test_setter_guards_default_message(setter):
    """Each setter must reject missing/too short default messages."""
    body = _function_body(_read_source(), setter)

    guard = GUARD_PATTERN.search(body)
    assert guard is not None, f"{setter} is missing the heatpump_default_message_ guard"
    guard_end = guard.end()
    assert "return;" in body[guard_end:], f"{setter} guard does not return early"

    first_access = body.find("heatpump_default_message_[")
    assert (
        first_access == -1 or guard.start() < first_access
    ), f"{setter} accesses heatpump_default_message_ before the guard"


def test_curve_reads_fixed_indices_only_after_guard():
    """set_command_curve reads fixed curve indices and must be guarded first."""
    body = _function_body(_read_source(), "set_command_curve")

    guard = GUARD_PATTERN.search(body)
    assert guard is not None

    for index in (75, 78, 86, 93):
        access = body.find(f"heatpump_default_message_[{index}]")
        assert access == -1 or guard.start() < access
