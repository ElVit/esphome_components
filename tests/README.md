# ESPHome Components Tests

This directory contains tests for all custom ESPHome components in this repository.

## Directory Structure

```
tests/
├── requirements.txt          # Common Python dependencies for all tests
├── panasonic_heatpump/      # Tests for panasonic_heatpump component
│   ├── README.md
│   ├── test_panasonic_heatpump.yaml
│   ├── test_panasonic_heatpump_unit.py
│   └── test_panasonic_heatpump_integration.py
└── [other_component]/       # Tests for other components (future)
```

## Running Tests

### Install Dependencies
```bash
pip install -r tests/requirements.txt
```

### Run All Tests
```bash
pytest tests/ -v
```

### Run Tests for Specific Component
```bash
# Panasonic Heatpump
pytest tests/panasonic_heatpump/ -v

# Other components (when added)
pytest tests/[component_name]/ -v
```

### Run with Coverage
```bash
pytest tests/ -v --cov=components --cov-report=html
```

## Adding Tests for New Components

When adding a new component, create a subdirectory under `tests/` with the same name as your component:

1. Create directory: `tests/[component_name]/`
2. Add test files:
   - `test_[component_name].yaml` - ESPHome configuration for testing
   - `test_[component_name]_unit.py` - Python unit tests
   - `test_[component_name]_integration.py` - Integration tests
   - `README.md` - Component-specific test documentation
3. Create GitHub Actions workflow: `.github/workflows/test_[component_name].yml`

## Test Types

### Unit Tests
- Test Python configuration validation
- Test component metadata
- Test schema structure
- Fast execution, no compilation required

### Integration Tests
- Test ESPHome configuration validation
- Test actual firmware compilation
- Test various configuration scenarios
- Slower execution, requires ESPHome

## Configuration

Test configuration is managed in `pytest.ini` at the repository root.

## GitHub Actions

Each component should have its own GitHub Actions workflow in `.github/workflows/` that:
- Validates ESPHome configurations
- Compiles test firmware
- Runs Python unit tests
- Checks code formatting
- Reports coverage

See `panasonic_heatpump` for a complete example.
