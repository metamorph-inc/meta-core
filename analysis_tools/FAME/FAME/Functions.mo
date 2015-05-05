//
// USAGERIGHTS fame
//

within FAME;

package Functions

  impure function get_fault_amount
    input String fault_name;
    input Real default_value;
    output Real amount_value;
  external "C" amount_value = FAME_get_fault_amount(fault_name, default_value) annotation(Library="faultmode.o");
  end get_fault_amount;  

end Functions;    
      
           
