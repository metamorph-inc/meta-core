import os
import sys

def main():
    blast_config = 'BlastConfig.json'
    corrosion_config = 'CorrosionConfig.json'
    found = False
	print 'Attempting to change config JSON filename'
    for file in os.listdir(os.getcwd()):  
        if file == blast_config:
            found = True
            os.rename(file, corrosion_config)
    if found == False:
        raise Exception('Config JSON file not found! Can not proceed with analysis')
        
if __name__=='__main__':
    main()

        