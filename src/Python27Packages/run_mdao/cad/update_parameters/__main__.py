import logging
import run_mdao.cad


def update_parameters():
    """Update parameters for the run. The parameters are read from testbench_manifest.json."""
    log = run_mdao.cad.log = logging.getLogger()
    log.info('About to update parameters in CADAssembly.xml.')
    ltc = run_mdao.cad.TestBenchParameter_to_CadAssembly()
    with open('log/CADParamTestBenchMapping.log', 'w') as statusfile:
        ltc.modify_cad_assembly_file(statusfile)

    log.info('Wrote back to CADAssembly.xml')

if __name__ == '__main__':
    update_parameters()
