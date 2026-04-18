# Testing Documentation for Maidesite Desk Component

This directory contains comprehensive tests for the `maidesite_desk` ESPHome component.

## Test Structure

### Unit Tests (`test_maidesite_desk_unit.py`)

- Tests Python configuration validation
- Tests component metadata (CODEOWNERS, DEPENDENCIES, etc.)
- Tests configuration schema structure
- Tests all platform imports (sensor, number, button)
- Tests code generation logic

### Integration Tests (`test_maidesite_desk_integration.py`)

- Tests minimal feature configuration on ESP32 board
- Tests full feature configuration on ESP32 board
- Tests configuration on ESP32-S2 board
- Tests configuration on ESP32-C3 board

### Test Configuration Files

- `test_maidesite_desk_esp32s2.yaml` - ESP32-S2 Wemos S2 Mini
- `test_maidesite_desk_esp32c3.yaml` - ESP32-C3 mini (RISC-V)
- `test_maidesite_desk_mini.yaml` - Minimal ESP32 configuration for basic testing
- `test_maidesite_desk_full.yaml` - Comprehensive ESP32 configuration with all features

## Running Tests Locally

### Prerequisites

```bash
pip install -r ../requirements.txt
```

### Run All Tests

```bash
pytest tests/maidesite_desk/ -v
```

### Run Only Unit Tests

```bash
pytest tests/maidesite_desk/test_maidesite_desk_unit.py -v
```

### Run Only Integration Tests

```bash
pytest tests/maidesite_desk/test_maidesite_desk_integration.py -v
```

### Run with Coverage

```bash
pytest tests/maidesite_desk/ --cov=components.maidesite_desk --cov-report=html
```

## Test Coverage

The test suite covers:

- Component configuration validation
- All sensor entities (height_abs, height_pct, height_min, height_max, position_m1-m4)
- All number entities (height_abs, height_pct)
- All button entities (stop, step_up, step_down, goto_max, goto_min, goto_m1-m4, save_m1-m4)
- Multiple ESP32 board variants (ESP32, ESP32-S2, ESP32-C3)
- Minimal and full configuration scenarios

## CI/CD Integration

These tests are designed to run in GitHub Actions CI/CD pipelines. See `.github/workflows/test_maidesite_desk.yml` for the workflow configuration.
