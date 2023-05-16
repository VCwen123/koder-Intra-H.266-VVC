# koder Intra H.266 VVC

# tryby kodowania Intra
1) Tryb DC (Differential Coding): Ten tryb kodowania Intra przewiduje wartość piksela na podstawie średniej wartości pikseli wokół niego. Przewidywana wartość jest następnie różnicą między przewidywanym pikselem a rzeczywistym pikselem, co prowadzi do kodowania różnicowego.

2) Tryb Planarny: Ten tryb kodowania Intra opiera się na przewidywaniu wartości pikseli na podstawie linii granicznej pomiędzy blokiem a jego sąsiednimi blokami. Wartości pikseli są przewidywane na podstawie interpolacji linii granicznej.

3) Tryby kątowe: H.266/VVC wprowadza nowe tryby kodowania Intra oparte na przewidywaniu pikseli na podstawie kątów. Algorytmy przewidywania w tych trybach wykorzystują różne kąty, takie jak 0°, 45°, 90°, 135° itp., aby przewidzieć wartości pikseli w bloku.
