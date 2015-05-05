(defproject dom-fun-eg "0.1.0-SNAPSHOT"
  :description "examples for the domain functions for meta"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.5.1"]
                 [org.zeromq/jeromq "0.3.2"]
                 [org.clojure/data.json "0.2.4"]
                 [org.zeromq/cljzmq "0.1.4"
                  :exclusions [org.zeromq/jzmq]]]
  :main dom-fun-eg.core
  :target-path "target/%s"
  :profiles {
             :uberjar {:aot :all}
             :dev {:aot :all} } )


