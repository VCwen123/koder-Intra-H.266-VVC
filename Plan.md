
### 1. Importowanie i przetwarzanie danych wideo
* Zaimportuj dane wideo i podziel je na pojedyncze klatki.
### 2. Podział klatki na bloki
* Podziel każdą klatkę na bloki o stałym rozmiarze (na przykład 8x8 pikseli).
### 3. Predykcja Intra dla każdego bloku
* Dla każdego bloku wykonaj predykcję Intra. W najprostszym przypadku możesz używać tylko jednego trybu predykcji (np. poziomego lub pionowego).
### 4. Obliczenie błędu (residual)
* Odejmij blok predykcyjny od rzeczywistego bloku, aby uzyskać blok błędu (residual).
### 5. Transformacja (DCT)
* Zastosuj Dyskretną Transformację Kosinusową (DCT) do bloku błędu.
### 6. Kwantyzacja
* Zastosuj kwantyzację do wyników DCT, zaokrąglając je do najbliższych wartości w określonym zestawie.
### 7. Kodowanie Entropii
* Zakoduj kwantyzowane bloki i inne informacje (jak tryby predykcji) przy użyciu mechanizmu kodowania entropii, takiego jak CABAC.
### 8. Pakowanie w strumień bitów
* Ułóż wszystkie zakodowane informacje w strumień bitów, który można następnie przechować lub przesłać.
### 9. Zapis do pliku
* Zapisz wynikowy strumień bitów do pliku wideo.
