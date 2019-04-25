## Vykreslovátko obrázků a textu do spektrogramu

Pokud si myslíte, že taková věc je přece úplně k ničemu, tak tomu tak úplně není. Projekt má jednu důležitou úlohu - získat mi zápočet z C++. Jinak to samozřejmě úplně k ničemu je. Což je docela důležitá vlastnost zápočťáku z C++, fakt bych nechtěl aby mi spravoval zálohy program, který jsem si sám napsal v tomto hrozném jazyku...

### Použití

Program se konfiguruje v souboru `main.cpp`:

Základní možnosti konfigurace jsou proměnné:

* `output` - WAV soubor do kterého se zapíše výstup
* `length_secs` - délka výstupu v sekundách
* `samplerate` - počet samplů za sekundu (normálně se používá 48k)
* `min_frequency` a `max_frequency` - rozsah frekvencí ve kterého se zobrazí obrázek

Pak je potřeba předat tomu data obrázku. Vstupem je seznam rovných čar, které se mají vykreslit. Z bitmapy neumím udělat nic co vypadá rozumně (ono je audio takové trochu jednorozměrné a dvourozměrné obrázky se do něj dají kódovat jen omezeně dobře) a pokroucené čáry se mi nechtělo implementovat.

První možnost je zapsat čáry takto (jo, `make_vector` je moje funkce, protože s standardní knihovnou tohoto jazyka nejde ani vyrobit vektor se zadanými prvky)

```C++
let lines = make_vector(
        Line(Point(0, 0), Point(1, 1)),
        //   ^ odkud      ^ kam
        Line(Point(0, 0), Point(1, 0))
        //         ^x ^y
    );
```

Druhá možnost je si vygenerovat čáry z textu. Funkce `strings2lines` se dá `vector` řádků textu, které se mají nakreslit.

```C++
let lines = strings2lines(make_vector<string>("Prvni radek", "druhy radek"), 1);
```

### Spuštění

Existuje asi deset způsobů, jak tu věc zpustit, podle toho co chci aby to dělalo.

Když chci spustit program aby to vydalo výsledek, tak je asi nejlepší `clang++ main.cpp -std=c++17 -Wall -Wextra -pedantic; ./a.out`, případně to funguje i s přepínačem `-Ofast`.

Když se něco rozbije, tak ale předchozí způsob vůbec nefunguje, protože chybu zatluče nebo obfuskuje.

Na hledání compile-time chyb je tam skript `show_errors.sh`, který program zkompiluje Clangem i GCC a zobrazí concatované výstupy v `less`u. Takže pak neodjíždí obrazovka a navíc pravděpodobnost, že daný chyba bude říkat něco užitečného se o trochu zvyšuje.

Na hledání runtime chyb se hodí buď address sanitizer nebo memory sanitizer, záleží o co jde:
* Segfault a nebo nevím: zkusit jestli program projde s `clang++ main.cpp  -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer -g`
* Program háže výjimku: spustit s memory sanizerem. On má totiž false-positive při vyhazování výjimky, ale narozdíl od ní vypíše stack trace (fakt nechápu, proč to nemůže exception dělat by default...): `clang++ main.cpp  -std=c++17 -Wall -Wextra -pedantic -fsanitize=memory -fno-omit-frame-pointer -g`
* Jinak zase zkusit ten memory sanitizer, šáhnout si v tomto skvělém a moderním jazyku na neinicializovanou paměť je překvapivě jednoduché.
