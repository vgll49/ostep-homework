with open("valgrind_trace.txt", mode='r') as f:    
    with open("ls_trace.txt", mode='w') as w:

        for line in f:
            x = line.split(",")
            y = x[0].split()
            #print(x)
            
            if len(y) > 1:
                if y[1][0].isdigit():
                    string_add = f"0x{y[1]}"
                    
                    int_adress = int(string_add, 16)
                    shift = int_adress >> 12
                    w.write(f"{shift}\n")
                    
        print("--trace genereated--")