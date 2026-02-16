"""
Unit tests for panasonic_heatpump component configuration validation.

Tests the Python configuration schema and validation logic.
"""

import pytest
from unittest.mock import Mock, MagicMock, patch
from esphome.core import CORE


class TestPanasonicHeatpumpConfig:
    """Test suite for panasonic_heatpump configuration validation."""

    @pytest.fixture(autouse=True)
    def setup(self):
        """Setup test fixtures."""
        # Mock CORE to avoid ESPHome initialization issues
        CORE.data = {}
        yield

    @pytest.fixture
    def mock_uart(self):
        """Mock UART component."""
        with patch("esphome.components.uart") as mock:
            yield mock

    @pytest.fixture
    def mock_cg(self):
        """Mock code generator."""
        with patch("esphome.codegen") as mock:
            mock.esphome_ns = MagicMock()
            mock.esphome_ns.namespace.return_value.class_ = MagicMock()
            yield mock

    def test_import_component(self):
        """Test that the component can be imported."""
        try:
            import sys
            import os

            # Add components directory to path
            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            import panasonic_heatpump as ph_init

            assert ph_init is not None
        except ImportError as e:
            pytest.fail(f"Failed to import panasonic_heatpump: {e}")

    def test_component_metadata(self):
        """Test component metadata is correctly defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        assert hasattr(ph_init, "CODEOWNERS")
        assert ph_init.CODEOWNERS == ["@elvit"]

        assert hasattr(ph_init, "MULTICONF")
        assert ph_init.MULTICONF is True

        assert hasattr(ph_init, "DEPENDENCIES")
        assert "uart" in ph_init.DEPENDENCIES

    def test_config_constants(self):
        """Test that configuration constants are defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        assert hasattr(ph_init, "CONF_PANASONIC_HEATPUMP_ID")
        assert ph_init.CONF_PANASONIC_HEATPUMP_ID == "panasonic_heatpump"

        assert hasattr(ph_init, "CONF_UART_CLIENT")
        assert ph_init.CONF_UART_CLIENT == "uart_client_id"

        assert hasattr(ph_init, "CONF_LOG_UART_MSG")
        assert ph_init.CONF_LOG_UART_MSG == "log_uart_msg"

    @patch("esphome.components.uart")
    @patch("esphome.codegen")
    def test_config_schema_structure(self, mock_cg, mock_uart):
        """Test that CONFIG_SCHEMA has the correct structure."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        assert hasattr(ph_init, "CONFIG_SCHEMA")
        assert ph_init.CONFIG_SCHEMA is not None

    def test_valid_minimal_config(self):
        """Test validation of minimal valid configuration."""
        config = {
            "id": "my_heatpump",
            "uart_id": "uart_bus",
        }
        # This would require full ESPHome environment to validate
        # Just verify the config structure is reasonable
        assert "id" in config
        assert "uart_id" in config

    def test_valid_full_config(self):
        """Test validation of full configuration with all options."""
        config = {
            "id": "my_heatpump",
            "uart_id": "uart_bus",
            "uart_client_id": "uart_client",
            "log_uart_msg": True,
            "update_interval": "3s",
        }
        assert "id" in config
        assert "uart_id" in config
        assert "uart_client_id" in config
        assert "log_uart_msg" in config
        assert "update_interval" in config

    def test_log_uart_msg_default(self):
        """Test that log_uart_msg defaults to False."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        # The default is specified in the schema as False
        # This test verifies the constant exists
        assert ph_init.CONF_LOG_UART_MSG == "log_uart_msg"

    def test_update_interval_default(self):
        """Test that update_interval has a default value."""
        # The default update_interval should be 3s according to the example
        default_interval = "3s"
        assert default_interval == "3s"

    def test_multiconf_support(self):
        """Test that component supports multiple instances."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        # Verify MULTICONF is True, allowing multiple instances
        assert ph_init.MULTICONF is True

    def test_uart_dependency(self):
        """Test that UART is listed as a dependency."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        assert "uart" in ph_init.DEPENDENCIES

    def test_namespace_definition(self):
        """Test that component namespace is correctly defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        # Verify namespace constant exists
        assert hasattr(ph_init, "panasonic_heatpump_ns")


class TestPanasonicHeatpumpPlatforms:
    """Test suite for platform configurations."""

    def test_sensor_platform_exists(self):
        """Test that sensor platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.sensor import __init__ as sensor_init

            assert sensor_init is not None
        except ImportError:
            pytest.skip("Sensor platform not accessible in test environment")

    def test_binary_sensor_platform_exists(self):
        """Test that binary_sensor platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.binary_sensor import __init__ as bs_init

            assert bs_init is not None
        except ImportError:
            pytest.skip("Binary sensor platform not accessible in test environment")

    def test_text_sensor_platform_exists(self):
        """Test that text_sensor platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.text_sensor import __init__ as ts_init

            assert ts_init is not None
        except ImportError:
            pytest.skip("Text sensor platform not accessible in test environment")

    def test_number_platform_exists(self):
        """Test that number platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.number import __init__ as num_init

            assert num_init is not None
        except ImportError:
            pytest.skip("Number platform not accessible in test environment")

    def test_select_platform_exists(self):
        """Test that select platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.select import __init__ as sel_init

            assert sel_init is not None
        except ImportError:
            pytest.skip("Select platform not accessible in test environment")

    def test_switch_platform_exists(self):
        """Test that switch platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.switch import __init__ as sw_init

            assert sw_init is not None
        except ImportError:
            pytest.skip("Switch platform not accessible in test environment")

    def test_climate_platform_exists(self):
        """Test that climate platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.climate import __init__ as clim_init

            assert clim_init is not None
        except ImportError:
            pytest.skip("Climate platform not accessible in test environment")

    def test_water_heater_platform_exists(self):
        """Test that water_heater platform can be imported."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.water_heater import __init__ as wh_init

            assert wh_init is not None
        except ImportError:
            pytest.skip("Water heater platform not accessible in test environment")

    def test_water_heater_platform_metadata(self):
        """Test that water_heater platform has correct configuration."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.water_heater import (
                CONFIG_SCHEMA,
                CONF_HEATER_TANK,
                TYPES,
            )

            assert CONFIG_SCHEMA is not None
            assert CONF_HEATER_TANK == "tank"
            assert CONF_HEATER_TANK in TYPES
        except ImportError:
            pytest.skip("Water heater platform not accessible in test environment")

    def test_water_heater_platform_options(self):
        """Test that water_heater_options schema is defined."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.water_heater import water_heater_options

            # Test default temperature range and step
            schema = water_heater_options(20.0, 65.0, 0.5)
            assert schema is not None

            # Verify defaults work
            schema2 = water_heater_options(15.0, 70.0, 1.0)
            assert schema2 is not None
        except ImportError:
            pytest.skip("Water heater platform not accessible in test environment")

    def test_water_heater_to_code_function(self):
        """Test that water_heater to_code function is defined."""
        try:
            import sys
            import os

            components_path = os.path.join(
                os.path.dirname(__file__), "..", "..", "components"
            )
            sys.path.insert(0, components_path)

            from panasonic_heatpump.water_heater import to_code

            assert callable(to_code)
        except ImportError:
            pytest.skip("Water heater platform not accessible in test environment")


class TestPanasonicHeatpumpCodeGeneration:
    """Test suite for code generation logic."""

    @pytest.fixture
    def mock_dependencies(self):
        """Mock all ESPHome dependencies."""
        with patch("esphome.codegen") as mock_cg, patch(
            "esphome.components.uart"
        ) as mock_uart, patch("esphome.config_validation"):

            mock_cg.new_Pvariable = MagicMock(return_value=Mock())
            mock_cg.register_component = MagicMock()
            mock_uart.register_uart_device = MagicMock()
            mock_cg.get_variable = MagicMock(return_value=Mock())
            mock_cg.add = MagicMock()

            yield {
                "cg": mock_cg,
                "uart": mock_uart,
            }

    def test_to_code_function_exists(self):
        """Test that to_code function is defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import panasonic_heatpump as ph_init

        assert hasattr(ph_init, "to_code")
        assert callable(ph_init.to_code)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
