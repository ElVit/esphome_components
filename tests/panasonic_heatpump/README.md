# Testing Documentation for Panasonic Heatpump Component

This directory contains comprehensive tests for the `panasonic_heatpump` ESPHome component.

## Test Structure

### Unit Tests (`test_panasonic_heatpump_unit.py`)
- Tests Python configuration validation
- Tests component metadata (CODEOWNERS, DEPENDENCIES, etc.)
- Tests configuration schema structure
- Tests all platform imports (sensor, binary_sensor, text_sensor, number, select, switch, climate)
- Tests code generation logic

### Integration Tests (`test_panasonic_heatpump_integration.py`)
- Tests ESPHome configuration validation
- Tests compilation of minimal configurations
- Tests multi-instance support
- Tests all sensor platforms together
- Tests climate platform configuration

### Test Configuration (`test_panasonic_heatpump.yaml`)
- Comprehensive YAML configuration testing all component features
- Tests all sensor types (top1-top46)
- Tests all binary sensors (top0-top7)
- Tests all text sensors (top4, top13, top17-top20, top26)
- Tests all number controls (set5-set9)
- Tests all select controls (set2-set4, set10-set14)
- Tests all switch controls (set1)
- Tests climate platform with heat and cool support

## Running Tests Locally

### Prerequisites
```bash
pip install -r tests/requirements.txt
```

### Run All Tests
```bash
pytest tests/panasonic_heatpump/ -v
```

### Run Only Unit Tests
```bash
pytest tests/panasonic_heatpump/test_panasonic_heatpump_unit.py -v
```

### Run Only Integration Tests
```bash
pytest tests/panasonic_heatpump/test_panasonic_heatpump_integration.py -v
```

### Run with Coverage
```bash
pytest tests/panasonic_heatpump/ -v --cov=components/panasonic_heatpump --cov-report=html
```

View coverage report by opening `htmlcov/index.html` in a browser.

### Run Specific Test
```bash
pytest tests/panasonic_heatpump/test_panasonic_heatpump_unit.py::TestPanasonicHeatpumpConfig::test_component_metadata -v
```

## GitHub Actions Workflow

The GitHub Actions workflow (`.github/workflows/test_panasonic_heatpump.yml`) includes:

### Jobs

1. **test-build**: Validates and compiles the component
   - Tests on multiple ESPHome versions (2024.11.0, latest)
   - Tests on multiple boards (esp32dev, lolin_s2_mini)
   - Caches pip dependencies for faster runs
   - Uploads build artifacts

2. **lint-code**: Runs clang-format on C++ code
   - Ensures consistent code formatting
   - Checks all .cpp and .h files

3. **lint-python**: Runs Python linters
   - black: Code formatting
   - flake8: Style guide enforcement

4. **test-python**: Runs Python unit tests
   - Executes pytest with coverage
   - Uploads coverage to Codecov

5. **test-minimal-config**: Tests minimal configuration
   - Ensures component works with bare minimum settings

6. **test-multiconf**: Tests multiple instances
   - Validates MULTICONF support

7. **summary**: Aggregates all test results
   - Fails if any test job fails
   - Provides overview of all test results

### Triggers

The workflow runs on:
- Push to `main` or `panasonic_dev` branches
- Pull requests to `main` branch
- Manual workflow dispatch
- When relevant files change:
  - `components/panasonic_heatpump/**`
  - `tests/panasonic_heatpump/**`
  - `.github/workflows/test_panasonic_heatpump.yml`

## Test Coverage

The test suite covers:

✅ Component configuration validation  
✅ UART dependency checking  
✅ Multi-instance support (MULTICONF)  
✅ All sensor platforms  
✅ All binary sensor platforms  
✅ All text sensor platforms  
✅ All number control platforms  
✅ All select control platforms  
✅ All switch control platforms  
✅ Climate platform with heating/cooling  
✅ Code compilation for ESP32 variants  
✅ C++ code formatting  
✅ Python code formatting and style  

## Configuration Examples

### Minimal Configuration
```yaml
uart:
  - id: uart_hp
    tx_pin: GPIO1
    rx_pin: GPIO3
    baud_rate: 9600
    parity: EVEN

panasonic_heatpump:
  id: hp
  uart_id: uart_hp
```

### Full Configuration
```yaml
uart:
  - id: uart_heatpump
    tx_pin: GPIO37
    rx_pin: GPIO39
    baud_rate: 9600
    parity: EVEN
  - id: uart_client
    tx_pin: GPIO18
    rx_pin: GPIO16
    baud_rate: 9600
    parity: EVEN

panasonic_heatpump:
  id: my_heatpump
  uart_id: uart_heatpump
  uart_client_id: uart_client
  log_uart_msg: true
  update_interval: 3s
```

### Multiple Instances
```yaml
panasonic_heatpump:
  - id: hp1
    uart_id: uart_hp1
    update_interval: 3s
  - id: hp2
    uart_id: uart_hp2
    update_interval: 5s
```

## Adding New Tests

When adding new features to the component:

1. Update `test_panasonic_heatpump.yaml` with new configuration options
2. Add unit tests in `test_panasonic_heatpump_unit.py`
3. Add integration tests in `test_panasonic_heatpump_integration.py` if needed
4. Update this README with new test coverage information

## Troubleshooting

### ESPHome Not Found
If you get "ESPHome not installed" errors, install it:
```bash
pip install esphome
```

### Import Errors
Make sure you're running pytest from the repository root:
```bash
cd /path/to/esphome_components
pytest tests/ -v
```

### Timeout Errors
Some tests may timeout on slower systems. Increase the timeout in `pytest.ini`:
```ini
timeout = 600  # 10 minutes
```

## Continuous Integration

All tests must pass before merging to main branch. The GitHub Actions workflow ensures:
- Code compiles successfully
- Code follows formatting standards
- All unit and integration tests pass
- Multiple ESPHome versions are supported
- Multiple ESP32 variants are supported

## Contributing

When contributing to the panasonic_heatpump component:
1. Write tests for new features
2. Ensure all existing tests pass
3. Maintain or improve code coverage
4. Follow the existing code style
5. Update this documentation as needed
