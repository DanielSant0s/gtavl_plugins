cp symbols.dat syms_backup.txt
ee-objdump -t relocator.elf | grep -e "load_plugin" -e "unload_erl" -e "erl_find_local_symbol" -e "erl_find_symbol" | awk '{printf "%s 0x%s\n", $6, substr($1, 3)}' > updates.txt
awk 'NR==FNR { updates[$1] = $2; next } 
     $1 in updates { print $1, updates[$1]; next } 
     { print }' updates.txt symbols.dat > temp && mv temp symbols.dat
grep -e "load_plugin" updates.txt
