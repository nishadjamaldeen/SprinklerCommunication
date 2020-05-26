def hex_formatter(text):
    output = ""
    text_list = text.split()

    
    for hx in text_list:
        output = output + "0x" + hx + ", "


    output = "{" + output[:-2] + "}"

    print(output)

hex_formatter("7E 00 10 10 01 00 13 A2 00 41 84 E9 BD FF FE 00 00 31 30 70")
