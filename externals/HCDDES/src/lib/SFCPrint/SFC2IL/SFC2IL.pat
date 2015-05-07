<:$Arg::combineUsage( usage1 : ocl::Integer ; usage2 : ocl::Integer ) : ocl::Integer defmethod :
{
  if usage1 = 3 then { 3; }
  else {
    if usage2 = 3 then { 3; }
    else {
      if usage1 = 0 then { usage2; }
      else {
        if usage2 = 0 then { usage1; }
        else {
          if usage2 = 2 then {
            if usage1 = 2 then { 2; } else { 3; } endif;
          } else {
            if usage1 = 1 then { 1; } else { 3; } endif;
  } endif;} endif;
        } endif;
      } endif;
    } endif;
}
$:>

<:$Arg::findCheckArgUsage( ) : ocl::Integer defmethod :
{
  if self.slab->size = 0 then { 0; } else { 1; } endif;
}
$:>

<:$Arg::findRefUsage( ) : ocl::Integer defmethod :
{
//print( "Arg::findRefUsage" );
  if self.ref->size = 0 then {
    0;
  } else {
    if self.ptr then {
      self.ref->iterate( r ; iterateUsage : ocl::Integer = 0 | {
        r.combineUsage( iterateUsage );
      });
    } else {
      1;
    } endif;
  } endif;
}
$:>

<:$Arg::findSetVarUsage( ) : ocl::Integer defmethod :
{
//print( "Arg::findSetVarUsage" );
  if self.setVar->size = 0 then { 0; }
  else {
    self.setVar->iterate( sv ; iterateUsage : ocl::Integer = 0 | {
      let setVarUsage : ocl::Integer =
       if self.ptr then {
         if sv.invert then { 2; } else { 1; } endif;
       } else {
         if sv.invert then { 0; } else { 1; } endif;
       } endif
      in {
        self.combineUsage( iterateUsage, setVarUsage );
      };
    });
  } endif;
}
$:>

<:$Arg::findUsage( ) : ocl::Integer defmethod :
{
  combineUsage(  findRefUsage(),  combineUsage( findCheckArgUsage(), findSetVarUsage() )  );
}
$:>

<:$Arg::printUsage( ) : bool defmethod :
{
  let usage : ocl::Integer = self.findUsage() in {
    if usage = 0 then { print( "neither" ); }
    else {
      if usage = 1 then { print( "in" ); }
      else {
        if usage = 2 then { print( "out" ); }
        else {
          print( "inout" );
        } endif;
      } endif;
    } endif;
  };
}
$:>

<:$Arg::processArg( ) : bool defmethod :
{
  if self.dt.isUndefined() then { printDefinitionNoDT( self.name, self.type, self.size ); }
  else                          { printDefinitionDT( self.name, self.dt ); } endif;
}
$:>


<:$ArgDeclBase::printDefinitionNoDT( name : ocl::String ; type : ocl::String ; size : ocl::Integer ) : bool defmethod :
{
  if size <= 1 then {
    print(  "scalar( '" + name + "', " + type + ", [] )"  );
  } else {
    print(  "vector( '" + name + "', " + type + ", [dim(0,"  );
    print( size - 1 );
    print(  ")], [] )"  );
  } endif;
}
$:>

<:$ArgDeclBase::printDefinitionDT( name : ocl::String ; dt : DT ) : bool defmethod :
{
  if dt.oclIsTypeOf( BasicType ) then {
    print(  "scalar( '" + name + "', " + dt.name + ", [] )"  );
  } else {
    if dt.oclIsTypeOf( Array ) then {
      let ar : Array = dt.oclAsType( Array ) in {
        let sq : ocl::Sequence = ar.getDimensions() in {
          if sq->size = 1 then {
            print(  "vector( '" + name + "', " + ar.getBaseType().name + ", [dim(0,"  );
            print(  sq.first().oclAsType( ocl::Integer ) - 1  );
            print(  ")], [] )"  );
          } else {
            if sq->size = 2 then {
              print(  "matrix( '" + name + "', " + ar.getBaseType().name + ", [dim(0,"  );
              print(  sq.first().oclAsType( ocl::Integer ) - 1  );
              print(  "), dim(0,"  );
              print(  sq.last().oclAsType( ocl::Integer ) - 1  );
              print(  ")], [] )"  );
            } else {
              print(  "array( '" + name + "', " + ar.getBaseType().name + ", [" );
              sq->iterate( n ; iter : bool = false | {
                if iter then { print( ", " ); } else { true; } endif;
                print( "dim(0," );
                print(  n.oclAsType( ocl::Integer ) - 1  );
                print( ")" );
                true;
              });
              print(  "], [] )"  );
            } endif;
          } endif;
        };
      };
    } else {
      print(  "struct( '" + name + "', '" + dt.name + "' )"  );
    } endif;
  } endif;
}
$:>


<:$ArgDeclRef::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if not( self.argdecl.isUndefined() ) && self.argdecl.oclIsKindOf( Declaration ) then {
    let dc : Declaration = self.argdecl.oclAsType( Declaration ) in {
      if dc.isGlobal() then {
        let usage : ocl::integer = self.findUsage() in {
          if type = 1 && ( usage = 1 || usage = 3 ) || type = 2 && usage >= 2 then {
            ocl::Set { dc };
          } else {
            ocl::Set { };
          } endif;
        };
      } else {
        ocl::Set { };
      } endif;
    };
  } else {
    ocl::Set { };
  } endif;
}
$:>

<:$ArgDeclRef::processArgDeclRef( ) : bool defmethod :
{
  if self.argdecl.isUndefined() then {
    print( self.unres );
  } else {

    print( "'" );

    let ad : ArgDeclBase = self.argdecl in {
      if ad.oclIsTypeOf( StateLabel ) then {
        print( ad.oclAsType( StateLabel ).name );
      } else {
        if ad.oclIsKindOf( Var ) then {
          print( ad.oclAsType( Var ).name );
        } else {
          print( ad.oclAsType( Arg ).name );
        } endif;
      } endif;
    };

    print( "'" );

  } endif;
}
$:>


<:$ArgVal::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.argexpr.isUndefined() then {
    ocl::Set { };
  } else {
    self.argexpr.getGlobals( type );
  } endif;
}
$:>

<:$ArgVal::findUsage( methodUsage : ocl::Integer ) : ocl::Integer defmethod :
{
  let ae : Exprs = self.argexpr in {
    if self.arg->size = 0 then {
      ae.combineUsage( methodUsage, 1 );
    } else {
      self.arg->iterate( ag ; iterateUsage : ocl::Integer = methodUsage | {
        ae.combineUsage( iterateUsage, ag.findUsage() );
      });
    } endif;
  };
}
$:>

<:$ArgVal::processArgVal( ) : bool defmethod :
{
  if self.argexpr.isUndefined() then {
    print( self.val );
  } else {
    self.argexpr.processExprs( "" );
  } endif;
}
$:>


<:$Array::getBaseType( ) : DT defmethod :
{
  let dt : DT = self.oclAsType( TypedEntity ).dt in {
    if dt.oclIsTypeOf( Array ) then {
      dt.oclAsType( Array ).getBaseType();
    } else {
      dt;
    } endif;
  };
}
$:>

<:$Array::getDimensions( ) : ocl::Sequence defmethod :
{
  let sq : ocl::Sequence = 
   if self.noelem <= 1 then {
     ocl::Sequence { };
   } else {
     ocl::Sequence { self.noelem };
   } endif
  in {
    let dt : DT = self.oclAsType( TypedEntity ).dt in {
      if dt.oclIsTypeOf( Array ) then {
        sq.union(  dt.oclAsType( Array ).getDimensions()  );
      } else {
        sq;
      } endif;
    };
  };
}
$:>


<:$BinaryExprs::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  self.leftexpr.getGlobals( type ).union(  self.rightexpr.getGlobals( type )  );
}
$:>

<:$BinaryExprs::getLeftUsage( ) : ocl::Integer defmethod :
{
  if self.op = "=" then {
    if not( self.argexprpar.isUndefined() ) then { self.argexprpar.findUsage( 2 ); } else { 2; } endif;
  } else {
    if self.op = "." then { self.findUsage(); } else { 1; } endif;
  } endif;
}
$:>

<:$BinaryExprs::getRightUsage( ) : ocl::Integer defmethod :
{
  if self.op = "." then { 0; } else { 1; } endif;
}
$:>

<:$BinaryExprs::isAssignmentOp( ) : bool defmethod :
{
  let op : ocl::String = self.op in {
    op = "="   ||
    op = "+="  || op = "-="  ||
    op = "*="  || op = "/="  || op = "%="  ||
    op = "&="  || op = "|="  || op = "^="  ||
    op = "<<=" || op = ">>=";
  };
}
$:>

<:$BinaryExprs::isPureOp( ) : bool defmethod :
{
  let op : ocl::String = self.op in {
    op = "+"  || op = "-"  ||
    op = "*"  || op = "/"  || op = "%"  ||
    op = "&"  || op = "|"  || op = "^"  ||
    op = "&&" || op = "||" ||
    op = "<<" || op = ">>" || op = ".";
  };
}
$:>

<:$BinaryExprs::isRelationalOp( ) : bool defmethod :
{
  let op : ocl::String = self.op in {
    op = "==" || op = ">=" || op = "<=" || op = ">" || op = "<";
  };
}
$:>

<:$BinaryExprs::processBinaryExprs( tag : ocl::String ) : bool defmethod :
{
//print( "BinaryExprs::processBinaryExprs" );

  // INDEXING OPERATOR
  if self.op = "[" then {
    print( "select( " );
    self.processArray();
    print( "] )" );
  } else {

    // NOT EQUALS
    if self.op = "!=" then {
      print( "not( ( " );
      self.leftexpr.processExprs( "" );
      print( " ) = ( " );
      self.rightexpr.processExprs( "" );
      print( " ) )" );
    } else {

      // RELATIONAL OPERATORS
      if self.isRelationalOp() then {
        self.leftexpr.processExprs( "" );
        print( " " + self.translateRelationalOp() + " " );
        self.rightexpr.processExprs( "" );

      } else {

        // ASSIGNMENT OPERATORS
        if self.isAssignmentOp() then {
          print( self.translateAssignmentOp() + "( " );
          self.leftexpr.processExprs( "" );
          print( ", " );
          self.rightexpr.processExprs( "" );
          print( ", [ mlabel( '" + tag + "' ) ] )" );
        } else {

          // PURE OPERATORS
          if self.isPureOp() then {
            print( self.translatePureOp() + "( " );
            self.leftexpr.processExprs( "" );
            print( ", " );
            self.rightexpr.processExprs( "" );
            print( " )" );
          } else {

          print( "UNKNOWN BINARY OPERATOR" );

  } endif;} endif;
        } endif;
      } endif;
    } endif;
}
$:>

<:$BinaryExprs::processArray( ) : bool defmethod :
{
  if self.op = "[" then {
    if self.leftexpr.oclIsTypeOf( BinaryExprs ) then {
      if self.leftexpr.oclAsType( BinaryExprs ).processArray() then { print( ", " ); } else { true; } endif;
      self.rightexpr.processExprs( "" );
      true;
    } else {
      self.leftexpr.processExprs( "" );
      print( ", [" );
      self.rightexpr.processExprs( "" );
      false;
    } endif;
  } else {
    self.processBinaryExprs( "" );
    print( ", [" );
    false;
  } endif;
}
$:>

<:$BinaryExprs::translateAssignmentOp( ) : ocl::String defmethod :
{
  let op : ocl::String = self.op in {
    if op = "=" then { "assign"; }
    else {
      if op = "|=" then { "assign_bit_or"; }
      else {
        if op = "^=" then { "assign_bit_xor"; }
        else {
          if op = "&=" then { "assign_bit_and"; }
          else {
            if op = ">>=" then { "assign_right_shift"; }
            else {
              if op = "<<=" then { "assign_left_shift"; }
              else {
                if op = "+=" then { "assign_plus"; }
                else {
                  if op = "-=" then { "assign_minus"; }
                  else {
                    if op = "*=" then { "assign_times"; }
                    else {
                      if op = "/=" then { "assign_div"; }
                      else {
                        if op = "%=" then { "assign_mod"; }
                        else {
                          "BAD ASSIGNMENT OPERATOR (" + op + ")";
        } endif;} endif;} endif;
      } endif;} endif;} endif;
    } endif;} endif;} endif;
          } endif;} endif;
  };
}
$:>

<:$BinaryExprs::translatePureOp( ) : ocl::String defmethod :
{
  let op : ocl::String = self.op in {
    if op = "+" then { "plus"; }
    else {
      if op = "-" then { "minus"; }
      else {
        if op = "*" then { "times"; }
        else {
          if op = "/" then { "div"; }
          else {
            if op = "%" then { "mod"; }
            else {
              if op = "<<" then { "left_shift"; }
              else {
                if op = ">>" then { "right_shift"; }
                else {
                  if op = "&&" then { "and"; }
                  else {
                    if op = "||" then { "or"; }
                    else {
                      if op = "&" then { "bit_and"; }
                      else {
                        if op = "|" then { "bit_or"; }
                        else {
                          if op = "^" then { "bit_xor"; }
                          else {
                            if op = "." then { "sselect"; }
                            else {
                              "BAD PURE OPERATOR (" + op + ")";
    } endif;} endif;} endif;} endif;
          } endif;} endif;} endif;
        } endif;} endif;} endif;
      } endif;} endif;} endif;
  };
}
$:>

<:$BinaryExprs::translateRelationalOp( ) : ocl::String defmethod :
{
  let op : ocl::String = self.op in {
    if op = "==" then { "="; }
    else {
      if op = "<=" then { "=<"; }
      else {
        op;
      } endif;
    } endif;
  };
}
$:>


<:$CheckArg::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.slab.isGlobal() && type = 1 then {
    ocl::Set { self.slab };
  } else {
    ocl::Set { } ;
  } endif;
}
$:>

<:$CheckArg::processCheckArg() : bool defmethod :
{
//print( "CheckArg::processCheckArg" );
  print ( "'" + self.slab.name + "' = '" + self.arg.name + "'" );
}
$:>


<:$CheckState::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if type = 1 then {

    let globals1 =
     if self.svar.isGlobal() then { ocl::Set { self.svar }; } else { ocl::Set { }; } endif
    in {

      let globals2 =
       globals1.union(
        if self.index.isGlobal() then { ocl::Set { self.index }; } else { ocl::Set { }; } endif
       )
      in {

        globals2.union( 
         if self.value.isGlobal() then { ocl::Set { self.value }; } else { ocl::Set { }; } endif
        );

    };};

  } else {

    ocl::Set { };

  } endif;
}
$:>

<:$CheckState::processCheckState( ) : bool defmethod :
{
//print( "CheckState::processCheckState" );

  let pn : bool = self.invert && not( self.andState ) || not( self.invert ) && self.andState in {

    if pn then { print( "not( "); } else { true; } endif;
    if self.andState then { print( "bit_and( " ); } else { true; } endif;

    print( "select( '" + self.svar.name + "',[ bit_and( '" + self.index.name + "', 0xFFFF ) ] )" );

    if self.andState then {
      print( ", right_shift( '" + self.value.name + "', 16 ) ) = 0" ); // END OF "bit_and( "
    } else {
      print( " = '" + self.value.name + "'" );
    } endif;

    if pn then { print( " )" ); } else { true; } endif; // END OF "not( "
  };
}
$:>


<:$CompoundStatement::getDeclarations( ) : ocl::Set defmethod :
{
  let localDcSet : ocl::set =
   self.stmnt->select(  s | s.oclIsKindOf( Declaration )  )->reject(  s | s.oclIsKindOf( DT )  )
  in {
    self.stmnt->select(  s | s.oclIsKindOf( CompoundStatement )  )->reject(  s | s.oclIsTypeOf( Function )  )->
     collect(  s | s.oclAsType( CompoundStatement )  )->iterate(  cs ; iterateDcSet : ocl::Set = localDcSet | {
      iterateDcSet.union( cs.getDeclarations() );
    });
  };
}
$:>

<:$CompoundStatement::getFunctions( ) : ocl::Set defmethod :
{
  let localFunctionSet : ocl::Set = self.stmnt->select(  s | s.oclIsTypeOf( Function )  ) in {
    self.stmnt->select(  s | s.oclIsKindOf( CompoundStatement )  )->select(   s | not(  s.oclIsTypeOf( Function )  )   )->
     collect(  s | s.oclAsType( CompoundStatement )  )->iterate(  cs ; iterateFunctionSet : ocl::Set = localFunctionSet | {
      iterateFunctionSet.union( cs.getFunctions() );
    });
  };
}
$:>

<:$CompoundStatement::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  let localGlobals : ocl::Set =
   self.stmnt->iterate( s ; iterateGlobals : ocl::Set = ocl::Set { } | {
     iterateGlobals.union(  s.getGlobals( type )  );
   })
  in {

    if self.oclIsTypeOf( IterativeBlock ) then {

      if not( self.oclAsType( IterativeBlock ).cond.isUndefined()  ) then {
        self.oclAsType( IterativeBlock ).cond->iterate( cn ; iterateGlobals : ocl::Set = localGlobals | {
          iterateGlobals.union(  cn.getGlobals( type )  );
        });
      } else {
        localGlobals;
      } endif;

    } else {

      if self.oclIsTypeOf( ConditionalBlock ) then {

        if not( self.oclAsType( ConditionalBlock ).cond.isUndefined()  ) then {
          self.oclAsType( ConditionalBlock ).cond->iterate( cn ; iterateGlobals : ocl::Set = localGlobals | {
            iterateGlobals.union(  cn.getGlobals( type )  );
          });
        } else {
          localGlobals;
        } endif;

      } else {

        localGlobals;

      } endif;
    } endif;
  };
}
$:>

<:$CompoundStatement::getPrograms( ) : ocl::Set defmethod :
{
  if self.oclIsTypeOf( Project ) then {
    self.oclAsType( Project ).stmnt->select(  s | s.oclIsTypeOf( Program )  );
  } else {

    if self.oclIsTypeOf( Program ) then {
      ocl::Set { self };
    } else {
      ocl::Set { };
    } endif;

  } endif;
}
$:>

<:$CompoundStatement::getStateLabels() : ocl::Set defmethod :
{
  let slSet : ocl::Set = self.stmnt->select(  s | s.oclIsTypeOf( StateLabel )  ) in {
    self.stmnt->select(  s | s.oclIsKindOf( CompoundStatement )  )->
     collect(  s | s.oclAsType( CompoundStatement )  )->
     iterate( cs ; iterateSLSet : ocl::Set = slSet | {
      iterateSLSet.union( cs.getStateLabels() );
    });
  };
}
$:>

<:$CompoundStatement::getStateLabels( programSet : ocl::Set ) : ocl::Set defmethod :
{
  programSet->collect(  cs | cs.oclAsType( CompoundStatement )  )->
   iterate( cs ; iterateSLSet : ocl::Set = ocl::Set { } | {
    iterateSLSet.union( cs.getStateLabels() );
  });
}
$:>

<:$CompoundStatement::notEmpty( ) : bool defmethod :
{
  if self.statementCount = 0 then {
    false;
  }  else {
    self.stmnt->iterate( s ; ne : bool = false | {
      ne || s.notEmpty();
    });
  } endif;
}
$:>

<:$CompoundStatement::printConditionSet( conditionSet : ocl::Set ) : bool defmethod :
{
  conditionSet->iterate( c ; iterateAnd : ocl::Integer = 1 | {
    if iterateAnd <> 1 then { print( "and( " ); } else { true; } endif;
    iterateAnd + 1;
  });

  conditionSet->iterate( c ; iterateAnd : ocl::Integer = 1 | {
    c.oclAsType( Condition ).processCondition();
    if iterateAnd <> 1  then { print( " )" ); } else { true; } endif;
    print( ", " );
    iterateAnd + 1;
  });

  true;
}
$:>

<:$CompoundStatement::printInit( programSet : ocl::Set ) : bool defmethod :
{
  let slSet : ocl::Set = getStateLabels( programSet ) in {
    if slSet->size = 0 then {
      print( "  []" );
    } else {

      print( "  " );
      if slSet->size = 1 then { print( "[" ); } else { print( "series( [" ); } endif;

        :>\n<:
        slSet->collect(  sl | sl.oclAsType( StateLabel )  )->iterate( sl ; comma : bool = false | {
          if comma then { print( "," ); :>\n<: } else { true; } endif;
          print( "    assign( " );
          print( sl.name + ", " );
          print( sl.value );
          print( ", [ mlabel( 'CONTEXT#" + sl.statementIndexString() + "' ) ] )" );
          true;
        });

      :>\n<:
      print( "  " );
      if slSet->size = 1 then { print( "]" ); } else { print( "], [] )" ); } endif;

    } endif;
  };
}
$:>

<:$CompoundStatement::printSystemFooter( ) : bool defmethod :
{
  print( "," );
  :>\n<:
  print( "[] )" ); :>\n<:
}
$:>

<:$CompoundStatement::printSystemHeader( filename : ocl::String ) : bool defmethod :
{
  print( "system( '" + filename + "'," ); :>\n<:
  print( "  [" ); :>\n<:
}
$:>

<:$CompoundStatement::printSystemInitHeader( ) : bool defmethod :
{
  :>\n<:
  print( "  ]," ); :>\n<:
}
$:>

<:$CompoundStatement::processInit( ) : bool defmethod :
{
//print( "CompoundStatment::processInit" );

  // LET THE FILENAME BE THE NAME OF THE FIRST PROGRAM (WITH THE ".il" EXTENSION).
  let programSet = getPrograms() in {
    let filename : ocl::String = programSet->collect(  s | s.oclAsType( CompoundStatement )  )->
     sortedBy( s | s.statementIndex )->
      first().oclAsType( Program ).filename
    in {
      open( filename + ".il", "o", handle );
      switch( handle );
      printSystemHeader( filename );
    };

    let leadingComma : bool = processStructs() in {
      let headerComma : bool = processProgramSet( programSet, leadingComma ) in {
        printSystemInitHeader();
        printInit( programSet );
    };};

    printSystemFooter();
  };
}
$:>


<:$CompoundStatement::processProgramSet(  programSet : ocl::Set ; leadingComma : bool  ) : bool defmethod :
{
  programSet->forAll( a | {

    let p : Program = a.oclAsType( Program ) in {
      let indent : ocl::String = "    " in {

        let svComma : bool = 
         p.stmnt->select(  s | s.oclIsTypeOf( StateVar )  )->collect(  s | s.oclAsType( StateVar )  )->
          iterate( sv ; comma : bool = leadingComma | {
           if comma then { print( ", " ); :>\n<: } else { true; } endif;
           print( indent );
           sv.processStateVar();
           true;
         })
        in {
    
          let lvComma : bool =
           p.stmnt->select(  s | s.oclIsTypeOf( LocalVar )  )->collect(  s | s.oclAsType( LocalVar )  )->
            iterate( lv ; comma : bool = svComma | {
             if comma then { print( ", " ); :>\n<: } else { true; } endif;
             print( indent );
             lv.processLocalVar();
             true;
           })
          in {
    
            let slComma : bool =
             p.stmnt->select(  s | s.oclIsTypeOf( StateLabel )  )->collect(  s | s.oclAsType( StateLabel )  )->
              iterate( sl ; comma : bool = lvComma | {
               if comma then { print( ", " ); :>\n<: } else { true; } endif;
               print( indent );
               sl.processStateLabel();
               true;
             })
            in {

              let functionSet : ocl::Set = p.getFunctions() in {
                functionSet->collect(  b | b.oclAsType( Function )  )->iterate(  f ; comma : bool = slComma | {
                  if f.statementCount <> -1 then {
                    if comma then { print( ", " ); :>\n<: } else { true; } endif;
                    f.processFunction( indent );
                    true;
                  } else {
                    comma;
                  } endif;
                });
    };};};};};};
  });
}
$:>

<:$CompoundStatement::processCompoundStatement( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "CompoundStatement::processCompoundStatement" );
  if self.oclIsTypeOf( ConditionalBlock ) then {
    self.oclAsType( ConditionalBlock ).processConditionalBlock( indent, tagPrefix );
  } else {

    if self.oclIsTypeOf( ConditionalGroup ) then {
      self.oclAsType( ConditionalGroup ).processConditionalGroup( indent, tagPrefix );
    } else {

      if self.oclIsTypeOf( IterativeBlock ) then {
        self.oclAsType( IterativeBlock ).processIterativeBlock( indent, tagPrefix );
      } else {

        print( "UNKNOWN COMPOUND STATEMENT" );

      } endif;
    } endif;
  } endif;
}
$:>

<:$CompoundStatement::processStructs( ) : bool defmethod :
{
  let structSequence : ocl::Sequence = processStructs( ocl::Sequence {} ) in {
    structSequence->collect(  a | a.oclAsType( Struct )  )->iterate( st ; comma : bool = false | {
      if comma then { print( "," ); :>\n<: } else { true; } endif;
      st.processStruct( "    " );
      true;
    });
    structSequence->size <> 0;
  };
}
$:>

<:$CompoundStatement::processStructs( methodStructSequence : ocl::Sequence ) : ocl::Sequence defmethod :
{
  let csStructSequence : ocl::Sequence =
   self.stmnt->select(  s | s.oclIsTypeOf( Struct )  )->collect(  s | s.oclAsType( Struct )  )->
    iterate( st ; iterateStructSequence : ocl::Sequence = methodStructSequence | {
     st.collectUnique( iterateStructSequence );
   })
  in {
    self.stmnt->select(  s | s.oclIsKindOf( CompoundStatement )  )->collect(  s | s.oclAsType( CompoundStatement )  )->
     iterate(  cs ; iterateStructSequence : ocl::Sequence = csStructSequence | {
      cs.processStructs( iterateStructSequence );
    });
  };
}
$:>

<:$CompoundStatement::series( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "CompoundStatement::series" );
  let sb : ocl::Integer = substance() in {
    if sb = 0 then {
      print( indent + "[]" );
    } else {

      if sb = 1 then {
        self.stmnt->select( s | s.notEmpty() )->forAll( s | { s.processStatement( indent, tagPrefix ); });
      } else {

        print( indent + "series( [" ); :>\n<:
        self.stmnt->select( s | s.notEmpty() )->sortedBy( s | s.statementIndex )->
         iterate( s ; comma : bool = false | {
          if comma then { print( "," ); :>\n<: } else { true; } endif;
          s.processStatement( indent + "  ", tagPrefix );
          true;
        });
        :>\n<:
        print( indent + "], [] )" );

      } endif;

    } endif;
  };
}
$:>

<:$CompoundStatement::substance( ) : ocl::Integer defmethod :
{
  self.stmnt->iterate( s ; sb : ocl::Integer = 0 | {
    if s.notEmpty() then { sb + 1; } else { sb; } endif;
  });
}
$:>


<:$Condition::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.oclIsTypeOf( UserCode ) then {
    self.oclAsType( UserCode ).getGlobals( type );
  } else {

    if self.oclIsTypeOf( CheckArg ) then {
      self.oclAsType( CheckArg ).getGlobals( type );
    } else {

      if self.oclIsTypeOf( CheckState ) then {
        self.oclAsType( CheckState ).getGlobals( type );
      } else {

        ocl::Set { };

      } endif;
    } endif;
  } endif;
}
$:>

<:$Condition::processCondition( ) : bool defmethod :
{
  if self.oclIsTypeOf( UserCode ) then {
    self.oclAsType( UserCode ).processUserCode( "" );
  } else {

    if self.oclIsTypeOf( CheckState ) then {
      self.oclAsType( CheckState ).processCheckState();
    } else {

      if self.oclIsTypeOf( CheckArg ) then {
        self.oclAsType( CheckArg ).processCheckArg();
      } else {
    
        print( "UNKNOWN CONDITION" );

      } endif;
    } endif;
  } endif;
}
$:>


<:$ConditionalBlock::printConsequent( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "ConditionalBlock::printConsequent" );

  print( "if( " );
  printConditionSet( self.cond );
  :>\n<:
  self.series( indent + "  ", tagPrefix );
}
$:>

<:$ConditionalBlock::processConditionalBlock( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "ConditionalBlock::processConditionBlock" );
  let tag : ocl::String = tagPrefix + "#" + statementIndexString() in {
    print( indent );
    printConsequent( indent, tag );
    print( ", " ); :>\n<:
    print( indent + "skip, [ mlabel( '" + tag + "' ) ] )" );
  };
}
$:>


<:$ConditionalGroup::processConditionalGroup( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "ConditionalGroup::processConditionalGroup" );

  let tag : ocl::String = tagPrefix + "#" + statementIndexString() in {
    let cbSet : ocl::Set = 
     self.stmnt->select( s | { s.oclIsTypeOf( ConditionalBlock ); })->select( s | s.notEmpty() )
    in {
      cbSet->collect(  s | s.oclAsType( ConditionalBlock )  )->sortedBy( cb | cb.statementIndex )->
       iterate( cb ; iterateIndent : ocl::String = indent | {
        print( iterateIndent );
        cb.printConsequent( iterateIndent, tag + "#" + cb.statementIndexString() );
        print( "," ); :>\n<:
        iterateIndent + "  ";
      });
  
      print( indent );
      cbSet->collect(  s | s.oclAsType( ConditionalBlock )  )->sortedBy( cb | 0 - cb.statementIndex )->
       iterate( cb ; sk : bool = false | {
        if sk then { true; } else { print( "skip" ); } endif;
        print( ", [ mlabel( '" + tag + "#" + cb.statementIndexString() + "' ) ] )" );
        true;
      });
  };};

}
$:>


<:$Declaration::processDeclaration( ) : bool defmethod :
{
//print( "Declaration::processDeclaration" );

  if self.oclIsTypeOf( StateVar ) then {
    self.oclAsType( StateVar ).processStateVar();
  } else {
    if self.oclIsTypeOf( StateLabel ) then {
      self.oclAsType( StateLabel ).processStateLabel();
    } else { // LocalVar
      self.oclAsType( LocalVar ).processLocalVar();
    } endif;
  } endif;

}
$:>


<:$Double::processDouble( ) : bool defmethod :
{
//print( "Double" );
  print( self.val );
}
$:>


<:$Exprs::combineUsage( methodUsage : ocl::Integer ) : ocl::Integer defmethod :
{
  self.combineUsage( methodUsage, self.findUsage() );
}
$:>

<:$Exprs::combineUsage( usage1 : ocl::Integer ; usage2 : ocl::Integer ) : ocl::Integer defmethod :
{
  if usage1 = 3 then { 3; }
  else {
    if usage2 = 3 then { 3; }
    else {
      if usage1 = 0 then { usage2; }
      else {
        if usage2 = 0 then { usage1; }
        else {
          if usage2 = 2 then {
            if usage1 = 2 then { 2; } else { 3; } endif;
          } else {
            if usage1 = 1 then { 1; } else { 3; } endif;
          } endif;
        } endif;
      } endif;
    } endif;
  } endif;
}
$:>

<:$Exprs::findUsage( ) : ocl::Integer defmethod :
{
  if (  not( self.codeexprpar.isUndefined() )  ) then {
    0;
  } else {

    if (  not( self.retexprpar.isUndefined() )  ) then {
      1;
    } else {

      if (  not( self.argexprpar.isUndefined() )  ) then {
        self.argexprpar.findUsage( 0 );
      } else {

        if (  not( self.subexprpar.isUndefined() )  ) then {
          self.subexprpar.getUsage();
        } else {

          if (  not( self.leftexprpar.isUndefined() )  ) then {
            self.leftexprpar.getLeftUsage();
          } else {

            if (  not( self.rightexprpar.isUndefined() )  ) then {
              self.rightexprpar.getRightUsage();
            } else {
              0;
            } endif;
          } endif;
        } endif;
      } endif;
    } endif;
  } endif;
}
$:>

<:$Exprs::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.oclIsTypeOf( BinaryExprs ) then {
    self.oclAsType( BinaryExprs ).getGlobals( type );
  } else {

    if self.oclIsTypeOf( UnaryExprs ) then {
      self.oclAsType( UnaryExprs ).getGlobals( type );
    } else {

      if self.oclIsTypeOf( FunctionCall ) then {
        self.oclAsType( FunctionCall ).getGlobals( type );
      } else {

        if self.oclIsTypeOf( ArgDeclRef ) then {
          self.oclAsType( ArgDeclRef ).getGlobals( type );
        } else {

          ocl::Set { };

        } endif;
      } endif;
    } endif;
  } endif;
}
$:>

<:$Exprs::processExprs( tag : ocl::String ) : bool defmethod :
{
//print( "Exprs::processExprs" );

  if (  self.oclIsTypeOf( Double )  ) then {
    self.oclAsType( Double ).processDouble();
  } else {

    if (  self.oclIsTypeOf( Int )  ) then {
      self.oclAsType( Int ).processInt();
    } else {

      if (  self.oclIsTypeOf( Str )  ) then {
        self.oclAsType( Str ).processStr();
      } else {

        if (  self.oclIsTypeOf( ArgDeclRef )  ) then {
          self.oclAsType( ArgDeclRef ).processArgDeclRef();
        } else {

          if (  self.oclIsTypeOf( FunctionCall )  ) then {
            self.oclAsType( FunctionCall ).processFunctionCall( tag );
          } else {

            if (  self.oclIsTypeOf( UnaryExprs )  ) then {
              self.oclAsType( UnaryExprs ).processUnaryExprs( tag );
            } else {

              if (  self.oclIsTypeOf( BinaryExprs )  ) then {
                self.oclAsType( BinaryExprs ).processBinaryExprs( tag );
              } else {

                true;

      } endif;} endif;
    } endif;} endif;
  } endif;} endif;
        } endif;
}
$:>

<:$Function::printArgs( indent : ocl::String ) : bool defmethod :
{
  print( indent + "[" );

  if self.arg->size = 0 then {
  print( "]," );
  } else {
    :>\n<:
    self.arg->sortedBy( a | a.argIndex )->iterate( a ; comma : bool = false | {
      if comma then { print( "," ); :>\n<: } else { true; } endif;
      print( indent + "  param( " );
      a.processArg();
      print( ", " );
      a.printUsage();
      print( " )" );
    });
    :>\n<:
    print( indent + "]," );
  } endif;
  print( " % PARAMETERS" ); :>\n<:

}
$:>

<:$Function::printGlobals( indent : ocl::String ) : bool defmethod :
{
  let globalIns : ocl::Set = getGlobalIns() in {
    let globalOuts : ocl::Set = getGlobalOuts() in {

      self.printGlobalSet( globalIns - globalOuts, indent );
      print( " % GLOBAL INS" ); :>\n<:

      self.printGlobalSet( globalOuts - globalIns, indent );
      print( " % GLOBAL OUTS" ); :>\n<:

      self.printGlobalSet(  globalIns.intersection( globalOuts ), indent  );
      print( " % GLOBAL INOUTS" ); :>\n<:
    };
  };
}
$:>

<:$Function::printGlobalSet( globals : ocl::Set ; indent : ocl::String ) : bool defmethod :
{
  print( indent + "[" );

  if globals->size = 0 then {
    print( "]," );
  } else {
    :>\n<:
    globals->iterate( g ; comma : bool = false | {
      if comma then { print( "," ); :>\n<: } else { true; } endif;

      print( indent + "  '" );

      if g.oclIsTypeOf( StateLabel ) then {
        print(  g.oclAsType( StateLabel ).name  );
      } else {
        print(  g.oclAsType( Var ).name  );
      } endif;

      print( "'" );
    });
    :>\n<:
    print( indent + "]," );
  } endif;

}
$:>

<:$Function::printLocals( indent : ocl::String ) : bool defmethod :
{
  print( indent + "[" );

  let dcSet = getDeclarations() in {
    if dcSet->size = 0 then {
      print( "]," );
    } else {
      :>\n<:
      dcSet->collect(  a | a.oclAsType( Declaration )  )->iterate( d ; comma : bool = false | {
        if comma then { print( "," ); :>\n<: } else { true; } endif;
        print( indent + "  " );
        d.processDeclaration();
      });
      :>\n<:
      print( indent + "]," );
    } endif;
    print( " % LOCALS" ); :>\n<:
  };

}
$:>

<:$Function::processFunction( indent : ocl::String ) : bool defmethod :
{
//print( "Function::processFunction" );

  let tag : ocl::String =
   if self.scope = "" then { self.name; } else { self.scope; } endif
  in {

    print( indent );
    if self.returnType = "void" then {
      print( "proc( '" + self.name + "'," );
    } else {
      print( "func( '" + self.name + "', " + self.returnType + "," );
    } endif;
    :>\n<:
 
    print( indent + "  decls(" );                    :>\n<:
    print( indent + "    [], % CONSTS" );            :>\n<:

    printArgs( indent + "    " );
    printLocals( indent + "    " );
    printGlobals( indent + "    " ); 

    print( indent + "  )," );                        :>\n<:
    series( indent + "  ", tag );                    :>\n<:
    print( indent + "[ mlabel( '" + tag + "' ) ] )" );
  };
}
$:>


<:$FunctionCall::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  self.argVal->iterate( av ; iterateGlobals : ocl::Set = ocl::Set { } | {
    iterateGlobals.union(  av.getGlobals( type )  );
  });
}
$:>

<:$FunctionCall::processFunctionCall( tagPrefix : ocl::String ) : bool defmethod :
{
//print( "FunctionCall::processFunctionCall" );

  let fn : Function = self.callee in {

    if fn.isUndefined() then {
      print( "fcall( '" );
      if self.libFuncName = "" then {
        print( "BAD_FUNCTION" );
      } else {
        print( self.libFuncName );
      } endif;
    } else {

      if fn.returnType = "void" then {
        print( "pcall( '" );
      } else {
        print( "fcall( '" );
      } endif;
      print( fn.name );

    } endif;

    print( "', [" );

    self.argVal->sortedBy( a | a.argIndex )->iterate( a ; comma : bool = false | {
      if comma then { print(", "); } else { true; } endif;
      a.processArgVal();
      true;
    });

    print( "]" );

    if not( fn.isUndefined() ) && fn.returnType = "void" then {
      print( ", [ mlabel( '" + tagPrefix + "#" + statementIndexString() + "' ) ]" );
    } else { true; } endif;

    print( " )" );
  };

  true;
}
$:>


<:$Int::processInt( ) : bool defmethod :
{
//print( "Int" );
  print( self.val );
}
$:>


<:$IterativeBlock::processIterativeBlock( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "IterativeBlock::processIterativeBlock" );

  let tag : ocl::String = tagPrefix + "#" + statementIndexString() in {
    print( indent + "while( " );

    printConditionSet( self.cond );
    :>\n<:
    self.series( indent + "  ", tag );

    print( ", " ); :>\n<:
    print( indent + "[ mlabel( '" + tag + "' ) ] )" );
  };
}
$:>


<:$LocalVar::processLocalVar( ) : bool defmethod :
{
  if self.dt.isUndefined() then { printDefinitionNoDT( self.name, self.type, self.size ); }
  else                          { printDefinitionDT( self.name, self.dt ); } endif;
}
$:>


<:$Return::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.retexpr.isUndefined() then {
    ocl::Set { };
  } else {
    self.retexpr.getGlobals( type );
  } endif;
}
$:>

<:$Return::processReturn( ) : bool defmethod :
{
  print( "return( " );
  if self.retexpr.isUndefined() then {
    print( self.val );
  } else {
    self.retexpr.processExprs( "" );
  } endif;
  print( ", [] )" );
}
$:>


<:$SetState::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if type = 1 then {
    let set1 : ocl::Set =
     if self.index.isGlobal() then { ocl::Set { self.index }; } else { ocl::Set { }; } endif
    in {
      set1.union(
       if self.value.isGlobal() then { ocl::Set { self.value }; } else { ocl::Set { }; } endif
      );
    };
  } else { type = 2;
    if self.svar.isGlobal() then { ocl::Set { self.svar }; } else { ocl::Set { }; } endif;
  } endif;
}
$:>

<:$SetState::processSetState( tagPrefix : ocl::String ) : bool defmethod :
{
//print( "SetState::processSetState" );

  if self.andState then {

    if self.invert then { print( "assign_bit_and( " ); } else { print( "assign_bit_or( " ); } endif;

    print( "select( '" + self.svar.name + "', [ bit_and( '" + self.index.name + "', 0xFFFF ) ] ), " );

    if self.invert then { print( "bit_neg( " ); } else { true; } endif;
    print( "right_shift( '" + self.value.name + "', 16 )" );
    if self.invert then { print( " )" ); } else { true; } endif;

  } else {

    print( "assign(  select( '" + self.svar.name + "', [ bit_and( '" + self.index.name + "', 0xFFFF ) ] ), " );

    if self.invert then { print( "-( " ); } else { true; } endif;
    print( "'" + self.value.name + "'" );
    if self.invert then { print( " )" ); } else { true; } endif;

  } endif;

  print( ", [ mlabel( '" + tagPrefix + "#" + statementIndexString() + "' ) ] )" );
}
$:>


<:$SetVar::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.lvar.isGlobal() && (  type = 1 && self.invert  ||  type = 2 && not( self.invert )  ) then {
    ocl::Set { self.lvar };
  } else {
    ocl::Set { };
  } endif;
}
$:>

<:$SetVar::processSetVar( tagPrefix : ocl::String ) : bool defmethod :
{
//print( "SetVar" );

  if self.invert then {
    print( "assign( '" + self.arg.name + "', '" + self.lvar.name );
  } else {
    print( "assign( '" + self.lvar.name + "', '" + self.arg.name );
  } endif;

  print( "', [ mlabel( '" + tagPrefix + "#" + statementIndexString() + "' ) ] )" );
}
$:>


<:$StateLabel::processStateLabel( ) : bool defmethod :
{
  print(  "scalar( '" + self.name + "', int, [] )"  );
}
$:>


<:$Statement::getGlobalIns( ) : ocl::Set defmethod :
{
  getGlobals( 1 );
}
$:>

<:$Statement::getGlobalOuts( ) : ocl::Set defmethod :
{
  getGlobals( 2 );
}
$:>

<:$Statement::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  // Declaration'S NOT NEEDED -- ONLY STATEMENTS THAT MAY CONTAIN ArgDeclRef's
  if self.oclIsKindOf( CompoundStatement ) then {
    self.oclAsType( CompoundStatement ).getGlobals( type );
  } else {
    if self.oclIsTypeOf( UserCode ) then {
      self.oclAsType( UserCode ).getGlobals( type );
    } else {

      if self.oclIsTypeOf( SetVar ) then {
        self.oclAsType( SetVar ).getGlobals( type );
      } else {

        if self.oclIsTypeOf( SetState ) then {
          self.oclAsType( SetState ).getGlobals( type );
        } else {

          if self.oclIsTypeOf( Return ) then {
            self.oclAsType( Return ).getGlobals( type );
          } else {

            if self.oclIsTypeOf( FunctionCall ) then {
              self.oclAsType( FunctionCall ).getGlobals( type );
            } else {

              ocl::Set { };

    } endif;} endif;
  } endif;} endif;
        } endif;
      } endif;
}
$:>

<:$Statement::isGlobal( ) : bool defmethod :
{
  not( self.csPar.isUndefined() ) && self.csPar.oclIsTypeOf( Program );
}
$:>

<:$Statement::notEmpty( ) : bool defmethod :
{
  if self.oclIsKindOf( CompoundStatement ) then {
    self.oclAsType( CompoundStatement ).notEmpty();
  } else {

    if self.oclIsKindOf( Declaration ) then {
      false;
    } else {

      if self.oclIsTypeOf( UserCode ) then {
        let uc : UserCode = self.oclAsType( UserCode ) in {
          not( uc.codeexpr.isUndefined() ) || uc.expr <> "";
        };
      } else {

        true;

      } endif;
    } endif;
  } endif;
}
$:>

<:$Statement::processStatement( indent : ocl::String ; tagPrefix : ocl::String ) : bool defmethod :
{
//print( "Statement::processStatement" );
  if self.oclIsKindOf( CompoundStatement ) then {
    self.oclAsType( CompoundStatement ).processCompoundStatement( indent, tagPrefix );
  } else {

    print( indent );

    if self.oclIsTypeOf( UserCode ) then {
      self.oclAsType( UserCode ).processUserCode( tagPrefix );
    } else {

      if self.oclIsTypeOf( FunctionCall ) then {
        self.oclAsType( FunctionCall ).processFunctionCall( tagPrefix );
      } else {

        if self.oclIsTypeOf( SetState ) then {
          self.oclAsType( SetState ).processSetState( tagPrefix );
        } else {

          if self.oclIsTypeOf( SetVar ) then {
            self.oclAsType( SetVar ).processSetVar( tagPrefix );
          } else {

            if self.oclIsTypeOf( Return ) then {
              self.oclAsType( Return ).processReturn();
            } else {

              true;

    } endif;} endif;
  } endif;} endif;
        } endif;
      } endif;
}
$:>

<:$Statement::statementIndexString( ) : ocl::String defmethod :
{
  let retval : ocl::String = self.statementIndexString( self.statementIndex ) in {
    if retval = "" then { "0"; } else { retval; } endif;
  };
}
$:>

<:$Statement::statementIndexString( index : ocl::Integer ) : ocl::String defmethod :
{
  if index = 0 then { ""; }
  else {
    statementIndexString( index div 10 ) + getDigitChar( index mod 10 );
  } endif;
}
$:>

<:$Statement::getDigitChar( digit : ocl::Integer ) : ocl::String defmethod :
{
  if digit = 0 then { "0"; }
  else {
    if digit = 1 then { "1"; }
    else {
      if digit = 2 then { "2"; }
      else {
        if digit = 3 then { "3"; }
        else {
          if digit = 4 then { "4"; }
          else {
            if digit = 5 then { "5"; }
            else {
              if digit = 6 then { "6"; }
              else {
                if digit = 7 then { "7"; }
                else {
                  if digit = 8 then { "8"; }
                  else {
                    if digit = 9 then { "9"; }
                    else {
                      "*";
    } endif;} endif;} endif;
  } endif;} endif;} endif;
        } endif;} endif;
      } endif;} endif;
}
$:>

<:$Statement::substance( ) : ocl::Integer defmethod :
{
  if self.notEmpty() then { 1; } else { 0; } endif;
}
$:>


<:$StateVar::processStateVar( ) : bool defmethod :
{
  if ( self.size <= 1 ) then {
    print(  "scalar( '" + self.name + "', int )"  );
  } else {
    print(  "vector( '" + self.name + "', int, [dim(0," );
    print( self.size - 1 );
    print( ")], [] )" );
  } endif;
}
$:>


<:$Str::processStr( ) : bool defmethod :
{
//print( "Str" );
  print( self.val );
}
$:>


<:$Struct::collectUnique(  methodStructSequence : ocl::Sequence ) : ocl::Sequence defmethod :
{
//print( "Struct::collectUnique" );

  if methodStructSequence.excludes( self ) && self.memberCount <> -1 then {

    // A STRUCT CANNOT, EITHER DIRECTLY OR INDIRECTLY, CONTAIN ITSELF.

    let localStructSequence : ocl::Sequence =
     self.memb->select(  lv | not( lv.dt.isUndefined() )  )->select(  lv | lv.dt.oclIsTypeOf( Struct )  )->
      collect(  lv | lv.dt.oclAsType( Struct )  )->iterate( st ; iterateStructSequence : ocl::Sequence = methodStructSequence | {
       st.collectUnique( iterateStructSequence );
     })
    in {

      localStructSequence.including( self );

    };

  } else {

    methodStructSequence;

  } endif;

}
$:>

<:$Struct::processStruct( indent : ocl::String ) : bool defmethod :
{
  print( indent + "struct( [], '" + self.name + "'" );
  self.memb->forAll(  lv | {
    print( ", " ); :>\n<: print( indent + "  " );
    lv.oclAsType( Declaration ).processDeclaration();
  });
:>\n<: print( indent + ")" );
  true;
}
$:>


<:$UnaryExprs::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  self.subexpr.getGlobals( type );
}
$:>

<:$UnaryExprs::getUsage( ) : ocl::Integer defmethod :
{
  // POST-INCREMENT/DECREMENT, REASONING
  // IF THE PARENT OF THE UnaryExprs IS UserCode, THEN UnaryExprs IS ONLY WRITTEN TO.
  // OTHERWISE, IT IS BOTH READ (BY DELAY DUE TO POST-INCREMENT/DECREMENT) FROM AS WELL AS WRITTEN TO.
  if self.op = "post++" || self.op = "post--" then {
    if not( self.codeexprpar.isUndefined() ) then { 2; } else { 3; } endif;
  } else {

    // PRE-INCREMENT/DECREMENT, REASONING
    // IF THE PARENT OF THE UnaryExprs IS UserCode, THEN UnaryExprs IS ONLY WRITTEN TO.
    // IF THE PARENT OF THE UnaryExprs IS ArgVal, THEN UnaryExprs MAY BE READ,
    //  DEPENDING ON USAGE OF ARGVAL IN CALLED FUNCTION.
    // OTHERWISE, IT IS BOTH READ FROM AS WELL AS WRITTEN TO.
    if self.op = "++" || self.op = "--" then {
      if not( self.codeexprpar.isUndefined() ) then {
        2;
      } else {
        if not( self.argexprpar.isUndefined() ) then {
          self.argexprpar.findUsage( 2 );
        } else {
          3;
        } endif;
      } endif;

    } else {

      // NO OPERATOR, DEFER TO PARENT Exprs
      if self.op = "" then {
        self.findUsage();
      } else {
        1;  // ANY OPERATOR OTHER THAN PRE/POST-INCREMENT/DECREMENT IS PURE, SO READ ONLY
      } endif;
    } endif;
  } endif;
}
$:>

<:$UnaryExprs::isDecrementOp( ) : bool defmethod :
{
  self.op = "--" || self.op = "post--";
}
$:>

<:$UnaryExprs::isIncrementOp( ) : bool defmethod :
{
  self.op = "++" || self.op = "post++";
}
$:>

<:$UnaryExprs::isSideEffectOp( ) : bool defmethod :
{
  isIncrementOp() || isDecrementOp();
}
$:>

<:$UnaryExprs::processUnaryExprs( tag : ocl::String ) : bool defmethod :
{
//print( "UnaryExprs::processUnaryExprs" );

  // ALL SIDE EFFECT OPERATORS ARE CURRENTLY PRE/POST INCREMENT/DECREMENT
  if self.isSideEffectOp() then {
    print( self.translateSideEffectOp() + "( " );
    self.subexpr.processExprs( "" );
    print( ", 1, [ mlabel( '" + tag + "' ) ] )" );
  } else {

    if self.op <> "" then { print( translateUnaryOp() + "( " ); } else { true; } endif;
    self.subexpr.processExprs( "" );
    if self.op <> "" then { print( " )" ); } else { true; } endif;

  } endif;

}
$:>

<:$UnaryExprs::translateSideEffectOp( ) : ocl::String defmethod :
{
  if isIncrementOp() then { "assign_plus"; }
  else {
    if isDecrementOp() then { "assign_minus"; }
    else {
      "BAD SIDE EFFECT OPERATOR (" + self.op + ")";
    } endif;
  } endif;
}
$:>

<:$UnaryExprs::translateUnaryOp( ) : ocl::String defmethod :
{
  let op : ocl::String = self.op in {
    if op = "~" then { "bit_neg"; }
    else {
      if op = "!" then { "not"; }
      else {
        if op = "+" then { "pos"; }
        else {
          op;
        } endif;
      } endif;
    } endif;
  };
}
$:>


<:$UserCode::getGlobals( type : ocl::Integer ) : ocl::Set defmethod :
{
  if self.codeexpr.isUndefined() then {
    ocl::Set { };
  } else {
    self.codeexpr.getGlobals( type );
  } endif;
}
$:>

<:$UserCode::processUserCode( tagPrefix : ocl::String ) : bool defmethod :
{
  if self.codeexpr.isUndefined() then {
    print( self.expr );
  } else {
    self.codeexpr.processExprs( tagPrefix + "#" + statementIndexString() );
  } endif;
}
$:>


<:
self.processInit();
:>
