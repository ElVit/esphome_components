"""
Integration tests for maidesite_desk component.

Tests the component's behavior with actual ESPHome compilation.
"""

import pytest
import subprocess
import os


class TestMaidesiteDeskIntegration:
    """Integration tests using ESPHome CLI."""

    @pytest.fixture
    def test_yaml_full(self):
        """Get the path to the full test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir, "tests", "maidesite_desk", "test_maidesite_desk_full.yaml"
        )

    @pytest.fixture
    def test_yaml_mini(self):
        """Get the path to the minimal test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir, "tests", "maidesite_desk", "test_maidesite_desk_mini.yaml"
        )

    @pytest.fixture
    def test_yaml_esp32s2(self):
        """Get the path to the ESP32-S2 test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir,
            "tests",
            "maidesite_desk",
            "test_maidesite_desk_esp32s2.yaml",
        )

    @pytest.fixture
    def test_yaml_esp32c3(self):
        """Get the path to the ESP32-C3 test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir,
            "tests",
            "maidesite_desk",
            "test_maidesite_desk_esp32c3.yaml",
        )

    def test_validate_full_config(self, test_yaml_full: str):
        """Test that the full configuration is valid."""
        if not os.path.exists(test_yaml_full):
            pytest.skip(f"Test file not found: {test_yaml_full}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_full],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"Full config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")

    def test_validate_mini_config(self, test_yaml_mini: str):
        """Test that the minimal configuration is valid."""
        if not os.path.exists(test_yaml_mini):
            pytest.skip(f"Test file not found: {test_yaml_mini}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_mini],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"Mini config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")

    def test_validate_esp32s2_config(self, test_yaml_esp32s2: str):
        """Test that the ESP32-S2 configuration is valid."""
        if not os.path.exists(test_yaml_esp32s2):
            pytest.skip(f"Test file not found: {test_yaml_esp32s2}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_esp32s2],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"ESP32-S2 config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")

    def test_validate_esp32c3_config(self, test_yaml_esp32c3: str):
        """Test that the ESP32-C3 configuration is valid."""
        if not os.path.exists(test_yaml_esp32c3):
            pytest.skip(f"Test file not found: {test_yaml_esp32c3}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_esp32c3],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"ESP32-C3 config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
