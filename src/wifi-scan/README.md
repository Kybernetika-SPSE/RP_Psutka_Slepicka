# Funkce wifi-scan

Tento projekt poskytuje nástroje pro skenování dostupných WiFi sítí pomocí ESP32 a jejich následné zpracování.

## Použití

1. Připojte ESP32 k počítači.
2. Spusťte `scan.py` pro inicializaci skenování.
3. Zadejte název a lokaci pro každé skenování.
4. Data budou uložena do `scanData/networks_raw.json`.
5. Spusťte `format.py` pro formátování dat do `scanData/networks.json`.

## Struktura souborů

- `scan.py`: Skript pro skenování WiFi sítí a ukládání dat.
- `format.py`: Skript pro formátování naskenovaných dat.
- `scanData/`: Složka obsahující naskenovaná data.
- `src/`: Zdrojové kódy pro ESP32.

## Poznámky

- Tento projekt vyžaduje knihovnu `pyserial` pro komunikaci s ESP32. Nainstalujte ji pomocí `pip install pyserial`.
- Ujistěte se, že ESP32 má potřebná oprávnění pro přístup k sériovému portu.
- Pravidelně zálohujte data skenování, abyste předešli jejich ztrátě.

Pro více informací se odkažte na hlavní dokumentaci projektu.