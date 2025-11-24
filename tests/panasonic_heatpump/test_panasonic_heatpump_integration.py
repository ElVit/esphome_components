"""
Integration tests for panasonic_heatpump component.

Tests the component's behavior with actual ESPHome compilation.
"""

import pytest
import subprocess
import os
import tempfile
import yaml


class TestPanasonicHeatpumpIntegration:
    """Integration tests using ESPHome CLI."""

    @pytest.fixture
    def test_yaml_path(self):
        """Get the path to the test YAML file."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(base_dir, 'tests', 'panasonic_heatpump', 'test_panasonic_heatpump.yaml')

    @pytest.fixture
    def components_dir(self):
        """Get the path to the components directory."""
        base_dir = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))
        return os.path.join(base_dir, 'components')

    @pytest.fixture
    def minimal_config(self, components_dir):
        """Create a minimal test configuration."""
        return {
            'external_components': [{
                'source': components_dir,
                'components': ['panasonic_heatpump']
            }],
            'esp32': {
                'board': 'esp32dev',
                'framework': {'type': 'esp-idf'}
            },
            'esphome': {
                'name': 'test-panasonic-minimal',
            },
            'logger': {},
            'uart': [{
                'id': 'uart_hp',
                'tx_pin': 'GPIO1',
                'rx_pin': 'GPIO3',
                'baud_rate': 9600,
                'parity': 'EVEN',
            }],
            'panasonic_heatpump': {
                'id': 'hp',
                'uart_id': 'uart_hp',
            }
        }

    def test_validate_test_config(self, test_yaml_path):
        """Test that the test configuration is valid."""
        if not os.path.exists(test_yaml_path):
            pytest.skip(f"Test file not found: {test_yaml_path}")
        
        try:
            result = subprocess.run(
                ['esphome', 'config', test_yaml_path],
                capture_output=True,
                text=True,
                timeout=120
            )
            assert result.returncode == 0, f"Config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")

    def test_compile_minimal_config(self, minimal_config):
        """Test compiling a minimal configuration."""
        with tempfile.NamedTemporaryFile(mode='w', suffix='.yaml', delete=False) as f:
            yaml.dump(minimal_config, f)
            temp_path = f.name

        try:
            result = subprocess.run(
                ['esphome', 'config', temp_path],
                capture_output=True,
                text=True,
                timeout=120
            )
            assert result.returncode == 0, f"Minimal config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")
        finally:
            os.unlink(temp_path)

    def test_multi_instance_config(self, components_dir):
        """Test configuration with multiple component instances."""
        config = {
            'external_components': [{
                'source': components_dir,
                'components': ['panasonic_heatpump']
            }],
            'esp32': {
                'board': 'esp32dev',
                'framework': {'type': 'esp-idf'}
            },
            'esphome': {
                'name': 'test-multi-panasonic',
            },
            'logger': {},
            'uart': [
                {
                    'id': 'uart_hp1',
                    'tx_pin': 'GPIO1',
                    'rx_pin': 'GPIO3',
                    'baud_rate': 9600,
                    'parity': 'EVEN',
                },
                {
                    'id': 'uart_hp2',
                    'tx_pin': 'GPIO16',
                    'rx_pin': 'GPIO17',
                    'baud_rate': 9600,
                    'parity': 'EVEN',
                }
            ],
            'panasonic_heatpump': [
                {
                    'id': 'hp1',
                    'uart_id': 'uart_hp1',
                    'update_interval': '3s',
                },
                {
                    'id': 'hp2',
                    'uart_id': 'uart_hp2',
                    'update_interval': '5s',
                }
            ]
        }

        with tempfile.NamedTemporaryFile(mode='w', suffix='.yaml', delete=False) as f:
            yaml.dump(config, f)
            temp_path = f.name

        try:
            result = subprocess.run(
                ['esphome', 'config', temp_path],
                capture_output=True,
                text=True,
                timeout=120
            )
            assert result.returncode == 0, f"Multi-instance config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")
        finally:
            os.unlink(temp_path)

    def test_all_sensors_config(self, components_dir):
        """Test configuration with all sensor types."""
        config = {
            'external_components': [{
                'source': components_dir,
                'components': ['panasonic_heatpump']
            }],
            'esp32': {
                'board': 'esp32dev',
                'framework': {'type': 'esp-idf'}
            },
            'esphome': {
                'name': 'test-all-sensors',
            },
            'logger': {},
            'uart': [{
                'id': 'uart_hp',
                'tx_pin': 'GPIO1',
                'rx_pin': 'GPIO3',
                'baud_rate': 9600,
                'parity': 'EVEN',
            }],
            'panasonic_heatpump': {
                'id': 'hp',
                'uart_id': 'uart_hp',
            },
            'sensor': [{
                'platform': 'panasonic_heatpump',
                'top1': {'name': 'Test Pump Flow'},
                'top5': {'name': 'Test Main Inlet Temp'},
                'top14': {'name': 'Test Outside Temp'},
            }],
            'binary_sensor': [{
                'platform': 'panasonic_heatpump',
                'top0': {'name': 'Test Heatpump State'},
            }],
            'text_sensor': [{
                'platform': 'panasonic_heatpump',
                'top4': {'name': 'Test Operating Mode State'},
            }],
        }

        with tempfile.NamedTemporaryFile(mode='w', suffix='.yaml', delete=False) as f:
            yaml.dump(config, f)
            temp_path = f.name

        try:
            result = subprocess.run(
                ['esphome', 'config', temp_path],
                capture_output=True,
                text=True,
                timeout=120
            )
            assert result.returncode == 0, f"All sensors config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")
        finally:
            os.unlink(temp_path)

    def test_climate_config(self, components_dir):
        """Test configuration with climate platform."""
        config = {
            'external_components': [{
                'source': components_dir,
                'components': ['panasonic_heatpump']
            }],
            'esp32': {
                'board': 'esp32dev',
                'framework': {'type': 'esp-idf'}
            },
            'esphome': {
                'name': 'test-climate',
            },
            'logger': {},
            'uart': [{
                'id': 'uart_hp',
                'tx_pin': 'GPIO1',
                'rx_pin': 'GPIO3',
                'baud_rate': 9600,
                'parity': 'EVEN',
            }],
            'panasonic_heatpump': {
                'id': 'hp',
                'uart_id': 'uart_hp',
            },
            'climate': [{
                'platform': 'panasonic_heatpump',
                'name': 'Test Climate',
                'supports_cool': True,
                'supports_heat': True,
            }],
        }

        with tempfile.NamedTemporaryFile(mode='w', suffix='.yaml', delete=False) as f:
            yaml.dump(config, f)
            temp_path = f.name

        try:
            result = subprocess.run(
                ['esphome', 'config', temp_path],
                capture_output=True,
                text=True,
                timeout=120
            )
            assert result.returncode == 0, f"Climate config validation failed: {result.stderr}"
        except FileNotFoundError:
            pytest.skip("ESPHome not installed")
        except subprocess.TimeoutExpired:
            pytest.fail("ESPHome config validation timed out")
        finally:
            os.unlink(temp_path)


if __name__ == '__main__':
    pytest.main([__file__, '-v'])
