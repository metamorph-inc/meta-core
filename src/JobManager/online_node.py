
import sys
import jenkinsapi

if __name__ == '__main__':
    from jenkinsapi.jenkins import Jenkins
    j = Jenkins('http://localhost:9999')
    for node in sys.argv[1:]:
        j.get_node(node).set_online()
