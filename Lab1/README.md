# Обработка изображения в формате PNM

Запуск программы:  
`CompGeomGraphics <input file name> <output file name> <command>`

Команды:
* 0 - инвертировать цвета  
![Initial image](../img/Lenna.ppm)
![Result](../img/Lenna_invert.ppm)

* 1 - отразить по горизонтали
* 2 - отразить по вертикали
* 3 - повернуть на 90 градусов по часовой стрелке
* 4 - повернуть на 90 градусов против часовой стрелки

Пример:  
`CompGeomGraphics Lenna.ppm output.ppm 0`