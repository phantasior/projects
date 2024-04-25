for file in src/*.c; do
    base_name=$(basename "$file" .c)
    clang -c "$file" -o "build/$base_name.o" -I src/ -DASN_DISABLE_OER_SUPPORT 
done