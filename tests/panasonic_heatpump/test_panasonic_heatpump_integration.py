"""
Integration tests for panasonic_heatpump component.

Tests the component's behavior with actual ESPHome compilation.
"""

import pytest
import subprocess
import os


class TestPanasonicHeatpumpIntegration:
    """Integration tests using ESPHome CLI."""

    @pytest.fixture
    def test_yaml_esp8266(self):
        """Get the path to the ESP8266 test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir,
            "tests",
            "panasonic_heatpump",
            "test_panasonic_heatpump_esp8266.yaml",
        )

    @pytest.fixture
    def test_yaml_esp32(self):
        """Get the path to the ESP32 test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir, "tests", "panasonic_heatpump", "test_panasonic_heatpump_full.yaml"
        )

    @pytest.fixture
    def test_yaml_esp32s2(self):
        """Get the path to the ESP32-S2 test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir,
            "tests",
            "panasonic_heatpump",
            "test_panasonic_heatpump_esp32s2.yaml",
        )

    @pytest.fixture
    def test_yaml_esp32c3(self):
        """Get the path to the ESP32-C3 test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir,
            "tests",
            "panasonic_heatpump",
            "test_panasonic_heatpump_esp32c3.yaml",
        )

    @pytest.fixture
    def test_yaml_cztaw1(self):
        """Get the path to the CZ-TAW1 client test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(
            base_dir,
            "tests",
            "panasonic_heatpump",
            "test_panasonic_heatpump_cztaw1.yaml",
        )

    def test_validate_esp8266_config(self, test_yaml_esp8266: str):
        """Test that the ESP8266 configuration is valid."""
        if not os.path.exists(test_yaml_esp8266):
            pytest.skip(f"Test file not found: {test_yaml_esp8266}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_esp8266],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"ESP8266 config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")

    def test_validate_esp32_config(self, test_yaml_esp32: str):
        """Test that the ESP32 configuration is valid."""
        if not os.path.exists(test_yaml_esp32):
            pytest.skip(f"Test file not found: {test_yaml_esp32}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_esp32],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"ESP32 config validation failed: {result.stderr}"
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

    def test_validate_cztaw1_config(self, test_yaml_cztaw1: str):
        """Test that the CZ-TAW1 client configuration with UART-proxy is valid."""
        if not os.path.exists(test_yaml_cztaw1):
            pytest.skip(f"Test file not found: {test_yaml_cztaw1}")

        try:
            result = subprocess.run(
                ["esphome", "config", test_yaml_cztaw1],
                capture_output=True,
                text=True,
                timeout=120,
            )
            assert (
                result.returncode == 0
            ), f"CZ-TAW1 config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
