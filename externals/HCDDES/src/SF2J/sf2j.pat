<:$Data::getDataType( ) : ocl::String defmethod :
{
  if self.typeBaseRef.isUndefined() then {
    self.DataType;
  } else {
    self.typeBaseRef.getTypeBaseRefType();
  } endif;
}
$:>

<:$Matrix::getMatrixType( ) : ocl::String defmethod :
{
  translateType( self.Type );
}
$:>

<:$Matrix::printMatrixDeclaration( varName : ocl::String ; indent : ocl::String ) : bool defmethod :
{
  print(  indent + "public " + translateType( getMatrixType() )  );
  if self.rows > 1 then { print( "[]" ); } else { true; } endif;
  if self.columns > 1 then { print( "[]" ); } else { true; } endif;
  print( " " + varName );
  if self.rows > 1 || self.columns > 1 then {
    print( " = new " );
    print( self.Type );
    if self.rows > 1 then { print( "[" ); print( self.rows ) ; print( "]" ); } else { true; } endif;
    if self.columns > 1 then { print( "[" ); print( self.columns ) ; print( "]" ); } else { true; } endif;
  } else { true; } endif;
  print( ";" ); :>\n<:
}
$:>

<:$Matrix::translateType( type : ocl::String ) : ocl::String defmethod :
{
//  if type = "int8" || type = "uint8" then { "byte"; }
  if type = "int8" || type = "uint8" then { "int"; }
  else {
//    if type = "int16" || type = "uint16" then { "short"; }
    if type = "int16" || type = "uint16" then { "int"; }
    else {
      if type = "int32" || type = "uint32" then { "int"; }
      else {
//        if type = "single" then { "float"; }
        if type = "single" then { "double"; }
        else {
          type;
        } endif;
      } endif;
    } endif;
  } endif;
}
$:>

<:$State::isInitialState( ) : bool defmethod :
{
  if self.state.Decomposition = "AND_STATE" then {
    true;
  } else {
    self.state.transConnector->select(  t | t.oclIsTypeOf( TransStart )  )->
     iterate( ts ; iterateInitial : bool = false | {
      iterateInitial || ts.dstTransition.includes( self );
    });
  } endif;
}
$:>

<:$State::composeStateNames( stateNames : ocl::Sequence ; startFromThis : bool ) : ocl::String defmethod :
{
//print( "State::composeStateNames" );
  let localStateNames : ocl::Sequence =
   if startFromThis then {
     stateNames;
   } else {
     if stateNames->size > 1 then {
       stateNames.subSequence( 1, stateNames->size - 1 );
     } else {
       ocl::Sequence { };
     } endif;
   } endif
  in {

    let localCompose : ocl::String =
     localStateNames->collect(  a | a.oclAsType( ocl::String )  )->
      iterate( s ; iterateCompose : ocl::String = "" | {
       if iterateCompose = "" then {
         if startFromThis then { s; } else { "_" + s; } endif;
       } else {
         iterateCompose + "._" + s;
       } endif;
     })
    in {
      if localCompose = "" then {
        let firstName : ocl::String = stateNames->first().oclAsType( ocl::String ) in {
          if firstName = self.getUniqueName() then { "this"; } else { firstName + ".this"; } endif;
        };
      } else {
        localCompose;
      } endif;
    };
  };
}
$:>

<:$State::getCommonTriggerTransitions( commonTrigger : ocl::String ; transitionSet : ocl::Set ) : ocl::Set defmethod :
{
  transitionSet->collect(  a | a.oclAsType( Transition )  )->iterate(  tr ; iterateSet : ocl::Set = ocl::Set { } | {
    if tr.Trigger = commonTrigger then { iterateSet.including( tr ); } else { iterateSet; } endif;
  });
}
$:>

<:$State::getLevel( ) : ocl::Integer defmethod :
{
  let parent : State = self.state in {
    if parent.isUndefined() then { 1; } else { parent.getLevel() + 1; } endif;
  };
}
$:>

// BUG IN SEQUENCES PREVENTS THEM FROM BEING RETURNED FROM A FUNCTION PROPERLY
<:$State::getAncestorNameSequence( ) : ocl::Sequence defmethod :
{
//print( "State::getAncestorNameSequence" );
  let parent : State = self.state in {
    if parent.isUndefined() then {
      ocl::Sequence { self.getUniqueName() };
    } else {
      parent.getAncestorNameSequence().append( self.getUniqueName() );
    } endif;
  };
}
$:>

<:$State::getNextState( rhs : State ) : ocl::String defmethod :
{
//print( "State::getNextState" );

  let lhsAncestorNames : ocl::Sequence = self.getAncestorNameSequence() in {
    let rhsAncestorNames : ocl::Sequence = rhs.getAncestorNameSequence() in {

      let localIndex : ocl::Integer =
       lhsAncestorNames->iterate(  a ; iterateIndex : ocl::Integer = 0 | {

         if iterateIndex > lhsAncestorNames->size ||
          iterateIndex > rhsAncestorNames->size ||
          lhsAncestorNames.at( iterateIndex ).oclAsType( ocl::String ) <>
           rhsAncestorNames.at( iterateIndex ).oclAsType( ocl::String )
         then {
           iterateIndex;
         } else {
           iterateIndex + 1;
         } endif;
       }) - 1

      in {
 
        if localIndex >= lhsAncestorNames->size then {
          if localIndex >= rhsAncestorNames->size then {
            "this";
          } else {
            composeStateNames(  rhsAncestorNames.subSequence( localIndex, rhsAncestorNames->size - 1 ), true  );
          } endif;
        } else {
          if localIndex >= rhsAncestorNames->size then {
            composeStateNames(  lhsAncestorNames.subSequence( localIndex, lhsAncestorNames->size - 1 ), true  );
          } else {
            composeStateNames(  rhsAncestorNames.subSequence( localIndex, rhsAncestorNames->size - 1 ), false  );
          } endif;
        } endif;

      };
    };
  };
}
$:>

<:$State::getOutput( ) : ocl::Set defmethod :
{
  let outputDataSet : ocl::Set =
   self.stateDE->select( sde1 | sde1.oclIsTypeOf( Data ) && sde1.oclAsType( Data ).Scope = "OUTPUT_DATA" ) in {

    if outputDataSet->size > 0 then {
      ocl::Set { outputDataSet.asSequence()->first().oclAsType( Data ) };
    } else {

      let outputEventSet : ocl::Set =
       self.stateDE->select( sde2 | sde2.oclIsTypeOf( Event ) && sde2.oclAsType( Event ).Scope = "OUTPUT_EVENT" ) in {

        if outputDataSet->size > 0 then {
          ocl::Set { outputEventSet.asSequence()->first().oclAsType( Event ) };
        } else {
          ocl::Set { };
        } endif;

      };

    } endif;
  };
}
$:>

<:$State::getUniqueName( ) : ocl::String defmethod :
{
  self.name + self.int2string( getLevel() );
}
$:>

<:$State::getLevel() : ocl::Integer defmethod :
{
  let parent : State = self.state in {
    if parent.isUndefined() then {
      0;
    } else {
      parent.getLevel() + 1;
    } endif;
  };
}
$:>

<:$State::int2string( index : ocl::Integer ) : ocl::String defmethod :
{
  if index = 0 then { ""; }
  else {
    int2string( index div 10 ) + getDigitChar( index mod 10 );
  } endif;
}
$:>

<:$State::getDigitChar( digit : ocl::Integer ) : ocl::String defmethod :
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

<:$State::printNextState( indent : ocl::String ; nextState : State ) : bool defmethod :
{
//print( "State::printNextState" );
  print(  indent + "setNextState( " + getNextState( nextState ) + " );"  ); :>\n<: 
}
$:>

<:$State::process( indent : ocl::String ) : bool defmethod :
{
  if self.Decomposition <> "FUNC_STATE" then {
    self.processState( indent );
  } else {
    self.processFunction( indent );
  } endif;
}
$:>

<:$State::processFunction( indent : ocl::String ) : bool defmethod :
{
  :>\n<:
  let output : ocl::Set = getOutput() in {

    print( indent + "public " );

    if output->size = 0 then {
      print( "void" );
    } else {
      let op : ocl::Any = output.asSequence()->first() in {
        if op.oclIsTypeOf( Data ) then {
          print(  op.oclAsType( Data ).getDataType()  );
        } else {
          print( "boolean" );
        } endif;
      };
    } endif;

    print( " " + self.name + "(" );

    if processFunctionArgs() then { print( " " ); } else { true; } endif;

    print( ") {" ); :>\n<:

    processFunctionLocals( indent + "  " );

    if output->size <> 0 then {
      :>\n<:
      let op : ocl::Any = output.asSequence()->first() in {
        if op.oclIsTypeOf( Data ) then {
          print(  indent + "  " + op.oclAsType( Data ).getDataType() + " " + op.oclAsType( Data ).name + " = 0;"  ); :>\n<:
        } else {
          print(  indent + "  " + "boolean " + op.oclAsType( Event ).name + " = false;"  ); :>\n<:
        } endif;
      };
    } else { true; } endif;

    processFunctionBody( indent + "  " );

    :>\n<:
    print( indent + "  return" );
    if output->size <> 0 then {
      let op : ocl::Any = output.asSequence()->first() in {
        print( " " );
        if op.oclIsTypeOf( Data ) then {
          print(  op.oclAsType( Data ).name  );
        } else {
          print(  op.oclAsType( Event ).name  );
        } endif;
      };
    } else {
      true;
    } endif;

    print( ";" ); :>\n<:

    print( indent + "}" ); :>\n<:

  };
}
$:>

<:$State::processFunctionBody( indent : ocl::String ) : bool defmethod :
{
  let ts : TransStart =
   self.transConnector->select(  t | t.oclIsTypeOf( TransStart )  ).asSequence()->first()
  in {
    ts.processFunctionActions( indent );
  };
}
$:>

<:$State::processFunctionArgs( ) : bool defmethod :
{
  // PRINT FUNCTION ARGUMENTS
  let dataComma : bool =
   self.stateDE->select(  sde | sde.oclIsTypeOf( Data )  )->collect(  sde | sde.oclAsType( Data )  )->
    select( d | d.Scope = "INPUT_DATA" )->sortedBy( d | -d.Port )->iterate( d ; comma = false | {
     if comma then { print( "," ); } else { true; } endif;
     print( " " + d.getDataType() + " " + d.name );
     true;
   })
  in {

    self.stateDE->select(  sde | sde.oclIsTypeOf( Event )  )->collect(  sde | sde.oclAsType( Event )  )->
     select( e | e.Scope = "INPUT_EVENT" )->sortedBy( e | e.Port )->iterate( e ; comma = dataComma | {
      if comma then { print( ", " ); } else { true; } endif;
      print( "boolean " + e.name );
      true;
    });

  };
}
$:>

<:$State::processFunctionLocals( indent : ocl::String ) : bool defmethod :
{
  // PRINT LOCAL FUNCTION VARIABLES
  let localDataSet : ocl::Bag = self.stateDE->select(  sde | sde.oclIsTypeOf( Data )  )->
   collect(  sde | sde.oclAsType( Data )  )->select( d | d.Scope = "LOCAL_DATA" ) in {
    if localDataSet->size > 0 then {
      :>\n<:
      localDataSet->collect(  a | a.oclAsType( Data )  )->forAll( d | {
        print( indent + d.getDataType() + " " + d.name + ";" ); :>\n<:
      });
    } else { true; } endif;
  };

  let localEventSet : ocl::Bag = self.stateDE->select(  sde | sde.oclIsTypeOf( Event )  )->
    collect(  sde | sde.oclAsType( Event )  )->select( e | e.Scope = "LOCAL_EVENT" ) in {
    if localEventSet->size > 0 then {
      :>\n<:
      localEventSet->collect(  a | a.oclAsType( Event )  )->forAll( e | {
        print( indent + "boolean " + e.name + ";" ); :>\n<:
      });
    } else { true; } endif;
  };
}
$:>

<:$State::processJunction( indent : ocl::String ; jn : Junction ) : bool defmethod :
{
//print( "State::processJunction" );

  jn.getDstTransitions()->collect(  a | a.oclAsType( Transition )  )->sortedBy( tr | tr.Order )->
   iterate( tr ; printElse = false | {

    if printElse then { print( " else " ); } else { print( indent ); } endif;

    print( "if ( " + performSubstitutions( tr.getAntecedent() ) + " ) {" ); :>\n<:

    if tr.ConditionAction <> "" then {
      print(  indent + "    " + processAction( tr.ConditionAction )  ); :>\n<:
    } else { true; } endif;
    if tr.Action <> "" then {
      print(  indent + "    " + processAction( tr.Action )  ); :>\n<:
    } else { true; } endif;

    if tr.dstTransition.oclIsTypeOf( State ) then {
      printNextState(  indent + "  ", tr.dstTransition.oclAsType( State )  );
    } else {
      processJunction(  indent + "  ", tr.dstTransition.oclAsType( Junction )  );
    } endif;

    print( indent + "}" );
  });
  :>\n<:
}
$:>

<:$State::processState( indent : ocl::String ) : bool defmethod :
{
  :>\n<:
  print( indent + "class " + self.getUniqueName() + " extends State {" ); :>\n<:

//  if self.event->size > 0 then {
//    :>\n<:
//    self.event->forAll( e | {
//      print( indent + "  boolean " + e.name + ";" ); :>\n<:
//    });
//  } else { true; } endif;

  if self.stateDE->select(  sde | sde.oclIsTypeOf( Data )  )->size > 0 then {
    :>\n<:
    self.stateDE->select(  sde | sde.oclIsTypeOf( Data )  )->collect(  sde | sde.oclAsType( Data )  )->forAll( d | {
      print( indent + "  " + d.getDataType() + " " + d.name );
      if d.InitialValue.size > 0 then {
        print( " = " );
        if d.getDataType() = "boolean" then {
          if d.InitialValue != "0" then { print( "true" ); } else { print( "false" ); } endif;
        } else {
          print( d.InitialValue );
        } endif;
      } else {
        print( " = " );
        if d.getDataType() = "boolean" then { print( "false" ); } else { print( "0" ); } endif;
      } endif;
      print( ";" );
      :>\n<:
    });
  } else { true; } endif;

  self.transConnector->select(  t | t.oclIsTypeOf( TransStart )  )->
   collect(  t | t.oclAsType( TransStart )  )->forAll(  ts | { ts.process( indent + "  " ); });

  self.transConnector->select(  t | t.oclIsTypeOf( State )  )->
   collect(  st | st.oclAsType( State )  )->forAll( st | { st.process( indent + "  " ); });

  if self.EnterAction <> "" then {
    :>\n<:
    print( indent + "  public void entryAction() {" ); :>\n<:
    print(  indent + "    " + processAction( self.EnterAction )  ); :>\n<:
    print( indent + "  }" ); :>\n<:
  } else { true; } endif;

  if self.DuringAction <> "" then {
    :>\n<:
    print( indent + "  public void doAction() {" ); :>\n<:
    print(  indent + "    " + processAction( self.DuringAction )  ); :>\n<:
    print( indent + "  }" ); :>\n<:
  } else { true; } endif;

  if self.ExitAction <> "" then {
    :>\n<:
    print( indent + "  public void exitAction() {" ); :>\n<:
    print(  indent + "    " + processAction( self.ExitAction )  ); :>\n<:
    print( indent + "  }" ); :>\n<:
  } else { true; } endif;

  processTransitions( indent + "  " );
  
  :>\n<:
  print( indent + "} " + self.getUniqueName() + " _" + self.getUniqueName() + " = " );

  let isInit : bool = isInitialState() in {
    if isInit then { print( "(" + self.getUniqueName() + ")makeInitial( " ); } else { true; } endif;
    print( "new " + self.getUniqueName() + "()" );
    if isInit then { print( " )" ); } else { true; } endif;
    print( ";" ); :>\n<:
  };
}
$:>

<:$State::processHighestState( indent : ocl::String ) : bool defmethod :
{
  print( "public class " + self.getUniqueName() + " extends State {" ); :>\n<:

  if self.stateDE->select(  sde | sde.oclIsTypeOf( Data )  )->size > 0 then {
    :>\n<:
    self.stateDE->select(  sde | sde.oclIsTypeOf( Data )  )->collect(  sde | sde.oclAsType( Data )  )->forAll( d | {
      print( indent + "  " + d.getDataType() + " " + d.name );
      if d.InitialValue.size > 0 then {
        print( " = " );
        if d.getDataType() = "boolean" then {
          if d.InitialValue != "0" then { print( "true" ); } else { print( "false" ); } endif;
        } else {
          print( d.InitialValue );
        } endif;
      } else {
        print( " = " );
        if d.getDataType() = "boolean" then { print( "false" ); } else { print( "0" ); } endif;
      } endif;
      print( ";" );
      if d.Scope.substring(0, 5) = "INPUT" then {
        print( " // INPUT" );
      } else {
        if d.Scope.substring(0, 6) = "OUTPUT" then {
          print( " // OUTPUT" );
        } else {
          true;
        } endif;
      } endif;
      :>\n<:
    });
  } else { true; } endif;

  self.transConnector->select(  t | t.oclIsTypeOf( TransStart )  )->
   collect(  t | t.oclAsType( TransStart )  )->forAll(  ts | { ts.process( indent + "  " ); });

  self.transConnector->select(  t | t.oclIsTypeOf( State )  )->
   collect(  t | t.oclAsType( State )  )->forAll(  st | { st.process( indent + "  " ); });

  print( "}" ); :>\n<:
}
$:>

<:$State::processTransitions( indent : ocl::String ) : bool defmethod :
{
  processTransitions( indent, getRefDstTransitions() );
}
$:>

<:$State::processTransitions( indent : ocl::String ; transitionSet : ocl::Set ) : bool defmethod :
{
//print( "State::processTransitions" );
  if transitionSet->size > 0 then {

    let firstTransition : Transition = transitionSet->collect(  a | a.oclAsType( Transition )  )->
     sortedBy( tr | tr.Order )->first() in {

      :>\n<:
      print( indent + "public void " );
      if firstTransition.Trigger = "" then { print( "completion" ); } else { print( firstTransition.Trigger ); } endif;
      print( "() {" ); :>\n<:

      let commonTriggerTransitions : ocl::Set = getCommonTriggerTransitions( firstTransition.Trigger, transitionSet ) in {

        print( indent + "  " );

        commonTriggerTransitions->collect(  a | a.oclAsType( Transition )  )->sortedBy( tr | tr.Order )->
         iterate( tr ; printElse = false | {

          if printElse then { print( " else " ); } else { true; } endif;

          print( "if ( " );
          if tr.Guard <> "" then { print(  performSubstitutions( tr.Guard )  ); } else { print( "true" ); } endif;
          print( " ) {" ); :>\n<:

          if tr.ConditionAction <> "" then {
            print(  indent + "    " + processAction( tr.ConditionAction )  ); :>\n<:
          } else { true; } endif;
          if tr.Action <> "" then {
            print(  indent + "    " + processAction( tr.Action )  ); :>\n<:
          } else { true; } endif;

          let tc : TransConnector = tr.dstTransition.getBaseTransConnector() in {
            if tc.oclIsTypeOf( State ) then {
              printNextState(  indent + "    ", tc.oclAsType( State )  );
            } else {
              processJunction(  indent + "    ", tc.oclAsType( Junction )  );
            } endif;
          };

          print( indent + "  }" );
          true;
        });
        :>\n<:
        print( indent + "}" ); :>\n<:

        processTransitions( indent, transitionSet - commonTriggerTransitions );
      };

    };

  } else { true; } endif;
}
$:>

<:$Stateflow::getHighestStates( ) : ocl::Set defmethod :
{
  self.state.asSequence()->first().transConnector->select(  tc | tc.oclIsTypeOf( State )  );
}
$:>


<:$TransConnector::find( str : ocl::String ; sbs : ocl::String ) : ocl::Integer defmethod :
{
  find( str, sbs, 0 );
}
$:>

<:$TransConnector::find( str : ocl::String ; sbs : ocl::String ; lowpos : ocl::Integer ) : ocl::Integer defmethod :
{
  let foo : ocl::Integer = find( str, sbs, lowpos, str.size - sbs.size ) in {
    foo;
  };
}
$:>

<:$TransConnector::find( str : ocl::String ; sbs : ocl::String ; lowpos : ocl::Integer ; highpos : ocl::Integer ) : ocl::Integer defmethod :
{
  if lowpos > highpos  then {
    -1;
  } else {
    if str.substring( lowpos, lowpos + sbs.size ) = sbs then {
      lowpos;
    } else {
      let halfway = ( highpos + lowpos ) div 2 in {
        let lowres = find( str, sbs, lowpos + 1, halfway ) in {
          if ( lowres <> -1 ) then {
            lowres;
          } else {
            find( str, sbs, halfway + 1, highpos );
          } endif;
        };
      };
    } endif;
  } endif;
}
$:>

<:$TransConnector::getAllDstTransitions( ) : ocl::Set defmethod :
{
  let parent : State = self.state in {
    if parent.isUndefined() then {
      ocl::Set { };
    } else {
      parent.getAllDstTransitions().union( getBaseDstTransitions() );
    } endif;
  };
}
$:>

<:$TransConnector::getBaseDstTransitions( ) : ocl::Set defmethod :
{
  let selfTransitions : ocl::Set = self.getDstTransitions() in {
    self.referedbyConnectorRef->iterate(  tc ; iterateTransitions : ocl::Set = selfTransitions | {
      iterateTransitions.union( tc.getBaseDstTransitions() );
    });
  };
}
$:>

<:$TransConnector::getBaseTransConnector( ) : TransConnector defmethod :
{
  if self.oclIsTypeOf( ConnectorRef ) then {
    self.oclAsType( ConnectorRef ).ref.getBaseTransConnector();
  } else {
    self;
  } endif;
}
$:>

<:$TransConnector::getDstTransitions( ) : ocl::Set defmethod :
{
  let parent : State = self.state in {
    if parent.isUndefined() then {
      ocl::Set { };
    } else {
      parent.transition->select( tr | tr.srcTransition = self );
    } endif;
  };
}
$:>

<:$TransConnector::getRefDstTransitions( ) : ocl::Set defmethod :
{
  self.getBaseTransConnector().getBaseDstTransitions();
}
$:>

<:$TransConnector::performSubstitutions( str : ocl::String ) : ocl::String defmethod :
{
//print( "performSubstitutions" );:
  performSubstitutions(  eliminateComments( str ), 0  );
}
$:>

<:$TransConnector::eliminateComments( str : ocl::String ) : ocl::String defmethod :
{
  let pos : ocl::Integer = find( str, "%", 0 ) in {
    if pos < 0 then {
      str;
    } else {
      str.substring( 0, pos );
    } endif;
  };
}
$:>

<:$TransConnector::performSubstitutions( str : ocl::String ; sta : ocl::Integer ) : ocl::String defmethod :
{
//print( "performSubstitutions,0" );
  let subPairs = ocl::Sequence {
   ocl::Sequence{ "~", "!" }
  } in {
    subPairs->iterate( sp ; iter : ocl::String = str | {
        let fsr : ocl::String = sp.first() in {
          let ssr : ocl::String = sp.last() in {
            let pos : ocl::Integer = find( str, fsr, sta ) in {
              if pos < 0 then {
                str;
              } else {
                performSubstitutions(
                 str.substring( 0, pos ).concat( ssr ).concat(  str.substring( pos + fsr.size, str.size )  ),
                 pos + ssr.size
                );
              } endif;
            };
          };
        };
    });
  };
}
$:>

<:$TransConnector::processAction( action : ocl::String ) : ocl::String defmethod :
{
  performSubstitutions(  terminateAction( action )  );
}
$:>

<:$TransConnector::processFunctionActions( indent : ocl::String ) : bool defmethod :
{
//print( "TransConnector::processFunctionActions" );
  let dstTransitions : ocl::Set = self.getDstTransitions() in {

    if dstTransitions->size > 0 then {

      :>\n<:
      print( indent );
      dstTransitions->collect(  a | a.oclAsType( Transition )  )->sortedBy( tr | tr.Order )->
       iterate( tr ; printElse = false | {

        if printElse then { print( " else " ); } else { true; } endif;

        print( "if ( " + performSubstitutions( tr.getAntecedent() ) + " ) {" ); :>\n<:
        if tr.ConditionAction <> "" then {
          print(  indent + "    " + processAction( tr.ConditionAction )  ); :>\n<:
        } else { true; } endif;
        if tr.Action <> "" then {
          print(  indent + "    " + processAction( tr.Action )  ); :>\n<:
        } else { true; } endif;

        tr.dstTransition->processFunctionActions( indent + "  " );

        print( indent + "}" );

        true;
      });
      :>\n<:

    } else { true; } endif;

  };
}
$:>

<:$TransConnector::terminateAction( action : ocl::String ) : ocl::String defmethod :
{
  let lastCharacter : ocl::String = action.substring( action.size - 1, action.size ) in {
    if lastCharacter = " " then {
      processAction(  action.substring( 0, action.size - 1 )  );
    } else {
      if lastCharacter <> ";" then { action + ";"; } else { action; } endif;
    } endif;
  };
}
$:>


<:$Transition::getAntecedent( ) : ocl::String defmethod :
{
  let antecedent : ocl::String = self.Trigger in {
    if self.Guard <> "" then {
      if antecedent <> "" then { antecedent + " && " + self.Guard; } else { self.Guard; } endif;
    } else {
      "true";
    } endif;
  };
}
$:>


<:$TransStart::makeInitialState( indent : ocl::String ; jn : Junction ) : bool defmethod :
{
  :>\n<:
  print( indent + "class Initial extends State {" ); :>\n\n<:
  print( indent + "  public void completion() {" ); :>\n<:
  
  processJunction( indent + "    ", jn );
  
  :>\n<:
  print( indent + "  }" ); :>\n<:
  print( indent + "} Initial _Initial = (Initial)makeInitial( new Initial() );" ); :>\n<:
}
$:>

<:$TransStart::process( indent : ocl::String ) : bool defmethod :
{
  let transitions : ocl::Set = getRefDstTransitions() in {

    transitions->collect(  tr | tr.oclAsType( Transition )  )->forAll( tr | {
      let tc : TransConnector = tr.dstTransition.getBaseTransConnector() in {
        if tc.oclIsTypeOf( Junction ) then {
           makeInitialState(  indent, tc.oclAsType( Junction )  );
        } else { true; } endif;
      };
    });

  };
}
$:>

<:$TransStart::processJunction( indent : ocl::String ; jn : Junction ) : bool defmethod :
{
//print( "TransStart::processJunction" );

  jn.getDstTransitions()->collect(  a | a.oclAsType( Transition )  )->sortedBy( tr | tr.Order )->
   iterate( tr ; printElse = false | {

    if printElse then { print( " else " ); } else { print( indent ); } endif;

    print( "if ( " + performSubstitutions( tr.getAntecedent() ) + " ) {" ); :>\n<:

    if tr.ConditionAction <> "" then {
      print(  indent + "    " + processAction( tr.ConditionAction )  ); :>\n<:
    } else { true; } endif;
    if tr.Action <> "" then {
      print(  indent + "    " + processAction( tr.Action )  ); :>\n<:
    } else { true; } endif;

    if tr.dstTransition.oclIsTypeOf( State ) then {
      print(  indent + "  setNextState( _" + tr.dstTransition.oclAsType( State ).getUniqueName() + " );"  ); :>\n<: 
    } else {
      processJunction(  indent + "  ", tr.dstTransition.oclAsType( Junction )  );
    } endif;

    print( indent + "}" );
  });
  :>\n<:
}
$:>

<:$TypeBase::getTypeBaseType( ) : ocl::String defmethod :
{
  if (  self.oclIsTypeOf( Matrix )  ) then {
    self.oclAsType( Matrix ).getMatrixType();
  } else {
    self.oclAsType( TypeStruct ).getTypeStructType();
  } endif;
}
$:>

<:$TypeBase::printTypeBaseDeclaration( varName : ocl::String ; indent : ocl::String ) : bool defmethod :
{
  if self.oclIsTypeOf( Matrix ) then {
    self.oclAsType( Matrix ).printMatrixDeclaration( varName, indent );
  } else {
    self.oclAsType( TypeStruct ).printTypeStructDeclaration( varName, indent );
  } endif;
}
$:>

<:$TypeBaseRef::getTypeBaseRefType( ) : ocl::String defmethod :
{
  self.ref.getTypeBaseType();
}
$:>

<:$TypeBaseRef::printTypeBaseRefDeclaration( indent : ocl::String ) : bool defmethod :
{
  self.ref.printTypeBaseDeclaration( self.name, indent );
}
$:>

<:$TypeStruct::getTypeStructType( ) : ocl::String defmethod :
{
  self.name;
}
$:>

<:$TypeStruct::printTypeStructDeclaration( varName : ocl::String ; indent : ocl::String ) : bool defmethod :
{
  print( indent + "public " + self.name + " " + varName + ";" ); :>\n<:
}
$:>

<:$Types::printStructs() : bool defmethod :
{
  self.typeBase->select(  t | t.oclIsTypeOf( TypeStruct )  )->collect(  t | t.oclAsType( TypeStruct )  )->forAll( ts | {
    open( ts.name + ".java", "o", handle );
    switch( handle );
    print( "public class " + ts.name + "{" ); :>\n<:
    ts.typeBaseRef->sortedBy( tbr | tbr.MemberIndex )->forAll( tbr | {
      tbr.printTypeBaseRefDeclaration( "  " );
    });
    print( "}" ); :>\n<:
  });
}
$:>

<:
  self.types.asSequence()->first().printStructs();

  self.designFolder->forAll( d | {
    d.modelsFolder->forAll( m | {
      let sf : Stateflow = m.stateflow.asSequence()->first() in {
        sf.getHighestStates()->collect(  hs | hs.oclAsType( State )  )->forAll( hs | {
          open( hs.getUniqueName() + ".java", "o", handle );
          switch( handle );

          hs.processHighestState( "" );
        });
      };
    });
  });
:>
