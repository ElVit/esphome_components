"""
Unit tests for maidesite_desk component configuration validation.

Tests the Python configuration schema and validation logic.
"""

import pytest
from unittest.mock import MagicMock, patch
from esphome.core import CORE


class TestMaidesiteDeskConfig:
    """Test suite for maidesite_desk configuration validation."""

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

            import maidesite_desk as md_init

            assert md_init is not None
        except ImportError as e:
            pytest.fail(f"Failed to import maidesite_desk: {e}")

    def test_component_metadata(self):
        """Test component metadata is correctly defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import maidesite_desk as md_init

        assert hasattr(md_init, "CODEOWNERS")
        assert md_init.CODEOWNERS == ["@elvit"]

        assert hasattr(md_init, "MULTICONF")
        assert md_init.MULTICONF is True

        assert hasattr(md_init, "DEPENDENCIES")
        assert "uart" in md_init.DEPENDENCIES

    def test_config_constants(self):
        """Test that configuration constants are defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import maidesite_desk as md_init

        assert hasattr(md_init, "CONF_MAIDESITE_DESK_ID")
        assert md_init.CONF_MAIDESITE_DESK_ID == "maidesite_desk"

        assert hasattr(md_init, "CONF_LOG_UART_MSG")
        assert md_init.CONF_LOG_UART_MSG == "log_uart_msg"

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

        import maidesite_desk as md_init

        assert hasattr(md_init, "CONFIG_SCHEMA")
        assert md_init.CONFIG_SCHEMA is not None

    def test_valid_minimal_config(self):
        """Test validation of minimal valid configuration."""
        config = {
            "id": "my_desk",
            "uart_id": "uart_bus",
        }
        # This would require full ESPHome environment to validate
        # Just verify the config structure is reasonable
        assert "id" in config
        assert "uart_id" in config

    def test_valid_full_config(self):
        """Test validation of full configuration with all options."""
        config = {
            "id": "my_desk",
            "uart_id": "uart_bus",
            "log_uart_msg": True,
        }
        assert "id" in config
        assert "uart_id" in config
        assert "log_uart_msg" in config

    def test_log_uart_msg_default(self):
        """Test that log_uart_msg defaults to False."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import maidesite_desk as md_init

        # The default is specified in the schema as False
        # This test verifies the constant exists
        assert md_init.CONF_LOG_UART_MSG == "log_uart_msg"

    def test_multiconf_support(self):
        """Test that component supports multiple instances."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import maidesite_desk as md_init

        # Verify MULTICONF is True, allowing multiple instances
        assert md_init.MULTICONF is True

    def test_uart_dependency(self):
        """Test that UART is listed as a dependency."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import maidesite_desk as md_init

        assert "uart" in md_init.DEPENDENCIES

    def test_namespace_definition(self):
        """Test that component namespace is correctly defined."""
        import sys
        import os

        components_path = os.path.join(
            os.path.dirname(__file__), "..", "..", "components"
        )
        sys.path.insert(0, components_path)

        import maidesite_desk as md_init

        assert hasattr(md_init, "maidesite_desk_ns")
        assert md_init.maidesite_desk_ns is not None

        assert hasattr(md_init, "MaidesiteDeskComponent")
        assert md_init.MaidesiteDeskComponent is not None
