import sys
import requests
import datetime
import json
import urlparse

if len(sys.argv) > 1:
    root_url = sys.argv[1]
else:
    root_url = 'http://localhost:8080/'
    #root_url = 'http://meta-vm0/'
    #root_url = 'http://kms1.isis.vanderbilt.edu:9999/'


node_prefix = 'template-test-slave'
num_new_slaves = 3

#slave_executors = 4
#slave_root_fs = r'c:\jenkins_builds'
#slave_labels = 'Windows13.1 OpenModelica py_modelica13.1 Windows12.12 Dymola PCC12.08'

username='zsolt'

def _request(method, *args, **kwargs):
    args = list(args)
    # TODO: configure for firewall/reverse proxy/etc
    #args[0] = args[0].replace(':8080', '')
    url = urlparse.urlparse(args[0])
    rurl = urlparse.urlparse(root_url)
    args[0] = urlparse.ParseResult(rurl.scheme, rurl.netloc, url.path, url.params, url.query, url.fragment).geturl()
    kwargs.setdefault('cookies', {})
    
    kwargs['cookies'].update(dict(username=username))
    kwargs.setdefault('headers', {})
    kwargs['headers']['X-Forwarded-User'] = username
    
    m = getattr(requests, method)
    ret = m(*args, **kwargs)
    if ret.status_code > 399:
        ret.raise_for_status()
    return ret

def get(*args, **kwargs):
    return _request('get', *args, **kwargs)

def post(*args, **kwargs):
    return _request('post', *args, **kwargs)

def get_build_time(url):
    t = get(url + 'buildTimestamp?format=yyyy/MM/dd%20HH:mm:ss')
    return datetime.datetime.strptime(t.content, '%Y/%m/%d %H:%M:%S')

def delete_job(url):
    post(url + 'doWipeOutWorkspace')
    post(url + 'doDelete') 

    
    
def main():
    root = get(root_url + "computer/api/json")
    template_node = None
    node_names = []
    for computer in json.loads(root.content)['computer']:
        node_names.append(computer['displayName'])
        if node_prefix == computer['displayName']:
            template_node= computer
    
    print node_names
    
    if not template_node:
        print node_prefix + ' was NOT found'
        return
    
    if node_prefix:
        for i in range(0, num_new_slaves):
            new_node_name = '{0}-{1}'.format(node_prefix, i)
            print new_node_name
            # if node exists delete first
            if new_node_name in node_names:
                print 'deleting first ' + new_node_name
                get(root_url + "computer/" + new_node_name + "/delete")
    
            with requests.session() as s: 
                print 'Sending create ' + new_node_name
                s.post(root_url + 'computer/createItem', {
                    'name':new_node_name,
                    'mode':'copy',
                    'from': node_prefix,
                    'json': {
                        "name": new_node_name, 
                        "mode": "copy",
                        "from": node_prefix,
                        "Submit": "OK"
                        }})

if __name__=='__main__':
    main()