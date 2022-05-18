# a
# sed 's/\./\.\n/g' breve_historia.txt > breve_historia_punto_a.txt

# b
# sed '/^\s$/d' breve_historia.txt > breve_historia_punto_b.txt

# c
# sed -e 's/\./\.\n/g' -e '/^\s$/d' breve_historia.txt > breve_historia_2.txt

# d
# grep -i ' guerra\b' breve_historia.txt

# e
# grep -E '^La.*(s|s\.)$' breve_historia.txt  # preguntar al profe

# f
# grep -c 'peronismo\b' breve_historia.txt

# g
# grep -c -E 'Sarmiento | Rosas' breve_historia.txt

# h SXIX = 1801 - 1900
# Muestre las oraciones que tengan fechas referidas al siglo XIX.
# grep -E '180[1-9] | 18[1-9][0-9] | 1900' breve_historia.txt
# grep -E '18(0[1-9] | [1-9][0-9]) | 1900' breve_historia.txt

# i
# sed -e 's/^\w*\b//g' breve_historia.txt > breve_historia_punto_i.txt

# j
ls | grep -c '.txt'

# --> consultar si el conteo es por palabra o por aparición en oración/párrafo
# --> consultar por punto h