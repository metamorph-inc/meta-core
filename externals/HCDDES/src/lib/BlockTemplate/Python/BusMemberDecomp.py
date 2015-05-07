from NamedBusMember import NamedBusMember

def getNamedBusMembers( argDeclBase ):

    name = argDeclBase.name;
    
    for namedBusMember in getBusMembersOfType( argDeclBase.type ):
        namedBusMember.prependToName( name )
        yield namedBusMember;

def getNamedBusMembersOfType( dt ):
    if dt.isStruct():
        return getBusNamedMembersOfStruct( dt )
    elif dt.isArray():
        return getBusNamedMembersOfArray( dt )
    elif dt.isBasicType():
        return getBusNamedMembersOfBasicType( dt )
    else:
        return

def getNamedBusMembersOfStruct( dt ):
    for member in dt.getMembers():
        for namedBusMember in getNamedBusMembers( member ):
            newNamedBusMember = namedBusMember
            newNamedBusMember.prependToName(  "." + str( member ) + busMember  )
            yield newNamedBusMember
           
def getNamedBusMembersOfArray( dt ):
    return NamedBusMember( dt )

def getNamedBusMembersOfBasicType():
    return NamedBusMember( dt )
