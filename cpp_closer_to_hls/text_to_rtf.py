def txt_to_rtf(txt_file_path, rtf_file_path):
    # Read the content of the text file
    with open(txt_file_path, 'r') as file:
        text_content = file.read()
    
    # RTF header and footer
    rtf_content = "{\\rtf1\\ansi\n" + text_content + "\n}"

    # Write to a new RTF file
    with open(rtf_file_path, 'w') as file:
        file.write(rtf_content)

# Usage
txt_to_rtf('hit_records.txt', 'hitrecords.rtf')
