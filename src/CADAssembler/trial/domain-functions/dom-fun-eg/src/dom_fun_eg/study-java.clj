
(defn dumpJava [sample]
   (require '[clojure.reflect :as r])
   (use '[clojure.pprint :only [print-table]])
   (print-table (:members (r/reflect sample))))
