

with open("valgrind_trace.txt", mode='r') as f:
    print(f.readline(100))
    lines = f.readlines()
    
    for line in lines:
       x = line.split(",")
       y = x[0].split()
       #print(x)
       
       if len(y) > 1:
            if y[1][0].isdigit():
                string_add = f"0x{y[1]}"
                
                int_adress = int(string_add, 16)
                shift = int_adress >> 12
                
                with open("ls_trace", mode='a') as w:
                    w.write(f"{shift},")
                    
    w.close()
    f.close()