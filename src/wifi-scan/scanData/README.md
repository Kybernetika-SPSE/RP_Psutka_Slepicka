# Složka s daty skenování

Tato složka obsahuje data skenování shromážděná nástrojem pro skenování WiFi. Datové soubory v tomto adresáři jsou generovány během procesu skenování a jsou používány pro analýzu a vytváření zpráv.

## Obsah

- `networks_raw.json`: Obsahuje surová data o sítích ve formátu JSON.
- `networks.json`: Obsahuje zpracovaná data o sítích ve formátu JSON.

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

## Použití

1. Spusťte ESP.
2. Spusťte `scan.py`.
3. Oskenujte libovolný počet lokací (u každé určíte vaši lokaci a název místa).
4. Ověřte, že vše proběhlo v pořádku v `networks_raw.json`.
5. Spusťte `format.py`.
6. Vezměte si svůj naformátovaný soubor `networks.json`.

## Poznámky

- Ujistěte se, že nástroj pro skenování má potřebná oprávnění k zápisu do tohoto adresáře.
- Pravidelně zálohujte data skenování, abyste předešli ztrátě dat.

Pro více informací se odkažte na hlavní dokumentaci nástroje pro skenování WiFi.