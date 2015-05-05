import os, fnmatch

def locate(pattern, root=os.curdir):
    '''Locate all files matching supplied filename pattern in and below
    supplied root directory.'''
    for path, dirs, files in os.walk(os.path.abspath(root)):
        for filename in fnmatch.filter(files, pattern):
            yield os.path.join(path, filename)

d_terms = {
    'RangeType': 'String',
    'RangeType?': 'String',
    'UnitType': 'String',
    'UnitType?': 'String',
    'Path': 'String',
    'URI': 'String',
    'URI?': 'String',
    'Real': 'Double',
    'Real?': 'Double?',
    'DistributionParameterType': 'String',
    'DistributionParameterType?': 'String',
    'ID': 'String',
    'HashType?': 'String',
    'HashType': 'String',
    'AVMID': 'String',
    'AVMID?': 'String',
    'Date': 'String',
    'Date?': 'String',
    'Integer?': 'int?',
    'Integer': 'int',
    'String?': 'String',
    'String[*]': 'String[]',
	'DimType?': 'String',
	'DimType': 'String',
    'XML': 'String'
}

def TransformFile(s_path):
    f = open(s_path, 'r')
    sa_code = f.readlines()
    f.close()

    b_changesInFile = 0
    for i in range(len(sa_code)):
        s_codeLine = sa_code[i]

        b_changesInLine = 0
        for k, v in d_terms.iteritems():
            s_find = ' ' + k + ' '
            s_replace = ' ' + v + ' '
            if s_codeLine.find(s_find) != -1:
                b_changesInLine += 1
            s_codeLine = s_codeLine.replace(s_find, s_replace)

            s_find = '<' + k + '>'
            s_replace = '<' + v + '>'
            if s_codeLine.find(s_find) != -1:
                b_changesInLine += 1
            s_codeLine = s_codeLine.replace(s_find, s_replace)

        if s_codeLine.find("public enum ") != -1 and s_codeLine.find(" : int") != -1:
            s_codeLine = s_codeLine.replace(" : int", "")
            b_changesInLine += 1

        if s_codeLine.find("::") != -1:
            s_codeLine = s_codeLine.replace("::", ".")
            b_changesInLine += 1

        if s_codeLine.find("IEnumerable") != -1:
            s_codeLine = s_codeLine.replace("IEnumerable", "List")
            b_changesInLine += 1

        if s_codeLine.find(": ValueType") != -1:
            s_codeLine = s_codeLine.replace(": ValueType", ": META.Design.ValueType")
            b_changesInLine += 1

        if b_changesInLine > 0:
            b_changesInFile += b_changesInLine
            sa_code[i] = s_codeLine

    if b_changesInFile > 0:
        f = open(s_path, 'w')
        f.writelines(sa_code)
        f.close()

if __name__ == "__main__":
    # We are going to get all *.cs files in "GeneratedCode" and
    #   do some find-and-replace in them

    print os.curdir

    for s_path in locate("*.cs", os.curdir + '\..\..\GeneratedCode'):
        TransformFile(s_path)