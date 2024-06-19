# PricingHelper
A small WIN32 program in c that has several tools to help with pricing formulas and basis values

## Formula Descriptions
- Multiplier: *0.67 -> Basis Value * 0.67
- Discount: -33 -> Basis Value * (1 + (-33 / 100))
- Markup: D0.75 -> Basis Value / 0.75
- Gross Profit: GP25 -> Basis Value / (1 - 25 / 100)

## The main window had three views
- Find Basis Value
- Find Formulas
- Formula Converter

### Find Basis Value View
- Inputs
    - Unit Price
    - Formula
- Outputs
    - Basis Value
- Buttons
    - Output has a button that copies that outputs value to the clipboard

### Find Formulas View
- Inputs
    - Unit Price
    - Bsais Value
- Outputs
    - Multiplier
    - Disocunt
    - Markup
    - Gross Profit
- Buttons
    - Each output has a button that copies that outputs value to the clipboard

### Formula Converter View
- Inputs
    - Formula
- Outputs
    - Multiplier
    - Discount
    - Markup
    - Gross Profit
- Buttons
    - Each output has a button that copies that outputs value to the clipboard