# a
sed 's/\./\.\n/g' breve_historia.txt

# b
sed '/^\s$/d' breve_historia.txt

# c
sed -e 's/\./\.\n/g' -e '/^\s$/d' breve_historia.txt > breve_historia_2.txt

# d
grep -i ' guerra\b' breve_historia.txt

# e
grep -E 'La.*(s\b|s\.)' breve_historia.txt

# f
grep -c 'peronismo\b' breve_historia.txt

# g
grep -c -E 'Sarmiento | Rosas' breve_historia.txt

# h
grep -E '((180[1-9]|18[1-9][0-9]|1900))[[\.|\,]]{0,1}' breve_historia.txt

# i
sed -e 's/^\w*\b//g' breve_historia.txt

# j
ls | grep -c '.txt'