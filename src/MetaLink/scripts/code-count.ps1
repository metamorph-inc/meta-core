

get-childitem -path . -filter "meta*" |
foreach-object -Process { 
   $cpp =  (get-childitem -path $_ -include *.h,*.cpp -recurse | select-string .).Count
   $java = (get-childitem -path $_ -include *.java -recurse | select-string .).Count
   $pb =  (get-childitem -path $_ -include *.proto -recurse | select-string .).Count
   write-host -NoNewline $_.Name 
   if ($cpp -gt 0) { write-host -NoNewline " c++: " $cpp  }
   if ($java -gt 0) { write-host -NoNewline " java: " $java } 
   if ($pb -gt 0) { write-host -NoNewline " protobuf: " $pb  }
   write-host
}
