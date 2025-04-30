# Složka s daty skenování

Tato složka obsahuje data naskenovaných WiFi sítí. Data jsou ukládána ve formátu JSON a jsou rozdělena na surová data a zpracovaná data. Surová data jsou uložena v souboru `networks_raw.json` a obsahují podrobné informace o každé nalezené síti. Zpracovaná data jsou uložena v souboru `networks.json` a obsahují agregované informace o síťových signálech.

## Obsah

- `networks_raw.json`: Obsahuje surová data o sítích ve formátu JSON.
- `networks.json`: Obsahuje zpracovaná data o sítích ve formátu JSON.

## Použití

1. Připojte ESP32 k počítači.
2. Spusťte `scan.py` pro inicializaci skenování.
3. Zadejte název a lokaci pro každé skenování.
4. Data budou uložena do `networks_raw.json`.
5. Spusťte `format.py` pro formátování dat do `networks.json`.


## Struktura souborů

### networks_raw.json

```json
[
    {
        "location": "test",
        "name": "PC",
        "networks": [
            {
                "bssid": "AE:15:A2:AA:80:22",
                "ssid": "pf-gosl",
                "rssi": -64
            },
            {
                "bssid": "AE:15:A2:AA:50:EA",
                "ssid": "pf-gosl",
                "rssi": -75
            }
        ]
    }
]
```

### networks.json

```json
{
    "PC": {
        "location": "test",
        "networks": {
            "AE:15:A2:AA:80:22": -69,
            "AE:15:A2:AA:50:EA": -72
        }
    }
}
```

## Poznámky

- Ujistěte se, že nástroj pro skenování má potřebná oprávnění k zápisu do tohoto adresáře.
- Pravidelně zálohujte data skenování, abyste předešli ztrátě dat.

Pro více informací se odkažte na hlavní dokumentaci nástroje pro skenování WiFi.