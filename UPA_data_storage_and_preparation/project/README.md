# UPA projekt: Zdravotnictví v ČR: část druhá
Cílem tohoto projektu je zpracovat a analyzovat data o obyvatelstvu ČR, poskytovatelů zdravotnických služeb a jejich vzájemný poměr v určitých oborech.

Autoři: 
- Magdaléna Ondrušková (xondru16)
- Daniel Paul (xpauld00)
- Monika Rosinská (xrosin00)

## Dotazy
Dle zadání máme provést následující analýzy:

- vytvořte sloupcové grafy zobrazující počty poskytovatelů určitého oboru pro Brno a zbytek Jihomoravského kraje (15 poskytovatelů)
- vytvořte čárový (spojnicový) graf zobrazující historii počtu poskytovatelů v 5 zvolených oborech (čtvrtletní hodnoty)
- sestavte žebříček krajů dle počtu obyvatel na jednoho praktického lékaře

Dále jsme si zvolili následující dotazy:

- vytvořte grafy zobrazující počet gynekologů na ženu v určitém kraji
- vytvořte grafy zobrazující počet lékáren v krajích

Dalším úkolem je připravit trénovací sadu pro klasifikátor, který na základě počtu poskytovatelů vybraných zdravotních služeb a věkového složení obyvatel odhadne velikost okresu (jako malý/střední/velký).

Výsledky dotazů i přípravy dat jsou uvedeny v souboru dokumentace_part_two.pdf.

## Spuštění aplikace
Jelikož ve druhé práci pracujeme se soubory vytvoření první části projektu, je potřeba nejdříve spustit i první část projektu.

Pro spuštění apliakce jsou potřeba následující nástroje a knihovny:

- python 3.8 nebo vyšší
- docker
- knihovna os
- virtualenv (na OS Linux 'pip install virtualenv')
- knihovna warnings

- knihovna pymongo (na OS Linux 'python -m pip install pymongo')
- knihovna pymongo[srv] (na OS Linux 'python -m pip install pymongo[srv]')
- knihovna pandas
- knihovna datetime
- knihovna csv
- knihovna json
- knihovna bson
- knihovna matplotlib
- knihovna seaborn
- knihovna numpy
- knihovna scipy
- knihovna unicodedata

#### Vytvoření virtualenv na operačním systému Linux
Nejdříve je potřeba aktivovat virtuální prostředí pomocí příkazu `virtualenv upa`, a poté překopírovat soubory z balíčku do nově vytvořeného adresáře (`source upa/bin/activate`).

#### Vytvoření virtualenv na operačním systému Windows
Nejdříve je potřeba nainstalovat virtualenv pomocou príkazu `pip install virtualenv`. Dále je potřeba vytvořit viruální prostředí pomocou `virtualenv upa`. Následně je potřeba aktivovat vytvořené virtuální prostředí pomocou `upa\Scripts\activate`. 

Zbylé kroky nezáleží nazvoleném operačním systému.

Knihovny potřebné pro druhou část projektu lze nainstalovat pomocí `pip install -r requirements.txt`.

Apliakce se spouští příkazem `python anal.py`

Po ukončení práce je třeba deaktivovat viruální prostředí pomocí příkazu `deactivate`.

Soubory projektu:
- anal.py - obsahuje zdrojový kód druhé části projektu
- requirements.txt - soubor obsahujíci potrebné python knihovny
- dokumentace.pdf - dokumentace k (druhé části) projektu
- DotazCPuvodni.csv
- DotazCUpraveny.csv
- README