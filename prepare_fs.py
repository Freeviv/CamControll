import os

def strip_spiffs_data():
    ftc = os.listdir('base_data')
    os.mkdir('data')
    for f in ftc:
        with open('base_data/' + f,'r') as fi:
            with open('data/' + f,'w+') as cf:
                for line in fi:
                    cf.write(line.strip() + '\n')


if __name__ == "__main__":
    strip_spiffs_data()
