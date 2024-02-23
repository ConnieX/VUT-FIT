# IPK projekt 1 - HTTP resolver doménových jmen
#### Autor: xrosin00

## Popis zadání:
Cílem projektu je implementace severu, který bude komunikovat protokolem HTTP a bude zajišťovat překlad doménových jmen. 
GET - převod host name na IP adresu a naopak (např. GET /resolve?name=apple.com&type=A HTTP/1.1 -> apple.com:A=17.142.160.59)
POST - v těle požadavku obsahuje seznam dotazů (každý na jiném řádku), řádek požadavku má formát POST /dns-query HTTP/1.1, a každý následující řádek má formát DOTAZ:TYP (např. 147.229.14.131:PTR)

# Spuštění: 
```sh
$ make
$ cd src
$ dotnet run arg1
```
nebo
```sh
$ make run PORT=xxxx
```
Aplikace požaduje při spuštění (alespoň) jeden argumet - číslo portu (tedy celé číslo v rozmezí 0 - 65 535). Případné další argumenty jsou ignorovány. Pokud není zadáno číslo portu, či je číslo ve špatném formátu, aplikace se ihned ukončí s příslušným komentářem.
## Řešení:
Projekt je napsán v jazyce C# pro .NET Core.
Kromě jiných jsem využila zejména třídu z namespace System.Net.Sockets, která umožňuje vytvoření socketu, a třídy z namespace System.Net, které umožňují například navázání komunikace s DNS serverem. 

Po úspěšném spuštění proběhne vytvoření socketu - pomocí localhost a čísla portu; typ socketu je Stream a je využíván TCP protokol.
Poté aplikace čeká na zaslání požadavku, poté vytvoří spojení s DNS serverem a následně přijatá data si uloží do pole bytů, které následně převede na text. Tento text je poté rozparsován a zpracován, je zkontrolováno, zda byla zadána správná metoda, a je i příslušného formátu. Poté probíhá komunikace s DNS serverem, a dle parametrů požadavku je pak uložena požadovaná informace. V případě, že je požadavkem získat IP adresu, je nalezena první IPv4. Poté je sestavena odpověď, která je následně překódována na byty a odeslána. Spojení se uzavírá a aplikace očekává další požadavek.
