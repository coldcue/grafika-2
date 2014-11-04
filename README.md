# A házi feladat leírása

Az alábbiakban olvashatod a Számítógépes Grafika tárgy kiadott házi feladatait.

## Második feladat

Készítsen csendéletet, ahol procedurálisan textúrázott, **diffúz**+**Blinn** spekuláris asztalon optikailag sima tárgyak, **egy ellipszoid aranykaktusz**, **egy paraboloid ezüstkaktusz** és **egy henger üvegkaktusz** áll (a háromból min. kettő megvalósítása szükséges). A kaktuszoktörzséből a törzzsel hasonló, de kisebb részek nőnek ki, mindig a felületre merőlegesen, legalább két további szinten (az összes részek számát úgy kell megválasztani, hogy a program 1 percen belül lefusson). A kinövő részek véletlenszerűen, nagyjából egyenletesen oszlanak el a felületen. A teret egy zöldes, egy kékes, és egy pirosas pontfényforrás világítja meg, a megvilágítási intenzitás a távolság négyzetével csökken. Az égbolt világoskék. Árnyékok vannak. A kamera az asztal fölött van és enyhén lefelé néz.

A sugár-objektum metszéspontot és a normálvektort a metszéspontban analitikusan kell számolni.

Az említett anyagok törésmutatója (n) és kioltási tényezője (k):

|               | r | g | b |
| ------------- | ----- | ----- | ----- |
| Üveg (n/k)  | 1.5/0.0 | 1.5/0.0 | 1.5/0.0 |
| Arany (n/k)  | 0.17/3.1 | 0.35/2.7 | 1.5/1.9 |
| Ezüst (n/k)  | 0.14/4.1 | 0.16/2.3 | 0.13/3.1|

*Beadási határidő: 2014. 11. 09. 23:59*

## A megoldás módja

A feladatot **C++** nyelven kell megvalósítani **OpenGL** és **GLUT** felhasználásával az alábbi sablon módosításával. A feladat megoldása során implementálni kell az `onInitialization()`, `onDisplay()`, `onKeyboard()`, `onMouse()`, `onMouseMotion()` és `onIdle()` függvényeket. Amennyiben a feladat megköveteli természetesen létrehozhatsz segédfüggvényeket is. Fontos azonban, hogy csak a jelzett részen belül található programrészeket értékeljük.

## A forráskód feltöltése

Az elkészült forráskód feltöltésére a Kódfeltöltés menüpont alatt van lehetőséged. A házi feladat határidejének lejártáig tetszőleges alkalommal tölthetsz fel megoldás, értékelni az utolsó változatot fogjuk.

A fordítási és futási eredmények ellenőrzése

A fordítás sikerességét az Eredmények menüpont alatt tudod ellenőrizni.

## Néhány tanács

A programokat GNU g++ 4.4.1 verziószámú fordítóprogrammal fordítjuk, mely szigorúbban követi a C++ szabványt mint a Visual Studio különböző változatai, ezért előfordulhat, hogy sikertelen lesz a fordítás annak ellenére, hogy a Visual Studio fordítójával lefordul. A pontos hibáról az Eredmények menüpont alatt tájékozódhatsz.

A feladatokat **ISO8859-1** vagy **ISO8859-2** formátumban várjuk, az **UTF-16** és **UNICODE** formátumú programok hibás fordítást okoznak.

A sablonban szereplő elválasztó sorokat (`//~~~~~~`...) célszerű a beküldött programban is megőrizni. Ellenkező esetben előfeldolgozási hiba lehet, bár a gyakoribb esetekre a parsert felkészítettük.

### Gyakori hibák

* Gyakori hiba konstans argumentumot átadni referenciát váró függvénynek. Pl. `void fv(int& a)` függvényt `void fv(2)`-vel hívni. Ez nem okoz hibát Visual Studio alatt az alap beállításokkal, de a szabvány szerint hibás.
* A tipikus **C++** hibákról jó összefoglalót találhatsz ezen az oldalon.
* Az **OpenGL** specifikáció nem rendelkezik a **ModelView** és **Projection** mátrixok alapértelmezett értékeiről. Ezek a program indulásakor tetszőlegesek lehetnek. Több beküldött feladatnál tapasztaltuk, hogy hibásan feltételezte az egység mátrixot. A kiadott forráskód-sablonban ezért inicializáljuk a mátrixokat a futás kezdetén.
* Több beküldött megoldásban tapasztaltuk az előfordítói `#define` direktívával definiált makrók hibás használatát. Semmi sem garantálja, hogy más rendszereken vagy akár csak más **GLUT** verzióban a numerikus konstansok megegyeznek, ezért hibás programozói gyakorlat ezen numerikus konstansok használata. Helyettük az előfordítói makrókat kell használni.
* Az `onIdle()` függvény akkor hívódik amikor semilyen esemény nem érkezik az rendszertől. Ennek megfelelően semmi sem garantálja, hogy mikor fog lefutni. Ebből következően itt olyan műveleteket végezni, melyek nélkül a renderelés hibás lesz (pl. a mátrixok beállítása) nem érdemes.
* Nehány hasznos tanács a **GLUT** használatához.
* Csak a sablonban regisztralt callback fuggvenyeket erdemes hasznalni, mivel a teszt rendszerben a tobbi glut fuggveny meghivasa sikertelen lesz.
