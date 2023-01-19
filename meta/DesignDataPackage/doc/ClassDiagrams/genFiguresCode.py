__author__ = 'adam'

import glob

for pdf in glob.glob("*.pdf"):
    nameroot = pdf.replace('.pdf','')
    print('\\begin{figure}[h!]')
    print('\\fbox{\includegraphics[width=\\textwidth]{ClassDiagrams/' + nameroot + '.pdf}}')
    if nameroot.find('_') >= 0:
        print('\\caption{' + nameroot.split('_')[0].replace('-','.') + ' Namespace: ' + nameroot.split('_')[1] + ' diagram}')
    else:
        print('\\caption{' + nameroot.replace('-','.') + ' Namespace}')
    print('\\end{figure}')
    print('')