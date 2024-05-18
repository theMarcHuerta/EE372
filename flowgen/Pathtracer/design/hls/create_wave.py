filename = '../../build/1-hls/build/Pathtracer.v1/concat_sim_rtl.v'  # replace with your Verilog file name

# read the Verilog file
with open(filename, 'r') as f:
    contents = f.readlines()

# find the index of the last occurrence of "endmodule"
endmodule_index = len(contents) - 1
while endmodule_index >= 0:
    if 'endmodule' in contents[endmodule_index]:
        break
    endmodule_index -= 1

# insert the lines before the last occurrence of "endmodule"
if endmodule_index >= 0:
    insert_lines = [
        'initial begin\n',
        '  $dumpfile("Conv.vcd");\n',
        '  $dumpvars(0, Conv);\n',
        '  #50000;\n',
        '  $finish(2);\n',
        'end\n'
    ]
    contents.insert(endmodule_index, '\n'.join(insert_lines))

# write the updated Verilog file
with open(filename, 'w') as f:
    contents = ''.join(contents)
    f.write(contents)