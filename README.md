# GPS(-szerű) program

## Pár szó a fájlokról
	A *.map kiterjesztésű fájlok egy elég egyszerűen jelölik a térkép terepét, a következőképp:
* A kettőskereszttel (#) jelzett területekre nem lehet lépni
* A ponttal (.) jelzett területek sima terepnek számít, a rálépés "időigénye" 3
* Egyenlőségjellel (=) vannak jelölve a gyorsfolgalmi utak, amiknek időigénye 2
* Kettősponttal jelöltem a nevezetes helyeket, amiknek a fájl alján van hozzárendelve egy településnév. Praktikussági szempontokból az időigénye szintén 2, így a súlyozott algoritmusban biztos átmegy rajta az útkereső

	