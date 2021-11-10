# GPS(-szerű) program

## Pár szó a fájlokról
	A *.map kiterjesztésű fájlok egy elég egyszerűen jelölik a térkép terepét, a következőképp:
* A kettőskereszttel (#) jelzett területekre nem lehet lépni
* A ponttal (.) jelzett területek sima terepnek számít, a rálépés "időigénye" 3
* Egyenlőségjellel (=) vannak jelölve a gyorsfolgalmi utak, amiknek időigénye 2
* Kettősponttal jelöltem a nevezetes helyeket, amiknek a fájl alján van hozzárendelve egy településnév. Praktikussági szempontokból az időigénye szintén 2, így a súlyozott algoritmusban biztos átmegy rajta az útkereső

## Koordináták száma
1. a fájl soronként vannak olvasva, ezért (x;y) [azaz (oszlop;sor)] koordináták helyett (y;x) [(sor;oszlop)] sorrenddel vannak a *.map fájlok jelölve
2. a sorszám az első méretet jelülő sor miatt eggyel lejjebb van csúszva 
	* pl. a (10;10) koordináta a fájl 11. sorában 10. karaktere (a tömb [10 - 2][10 - 1] indexe)

	