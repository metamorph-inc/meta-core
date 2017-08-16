import logging
import run_mdao.cad


def update_parameters():
    """Update parameters for the run. The parameters are read from testbench_manifest.json."""
    log = logging.getLogger()
    logging.basicConfig(level=logging.DEBUG)
    log.info('About to update parameters in CADAssembly.xml.')
    ltc = run_mdao.cad.TestBenchParameter_to_CadAssembly()
    ltc.modify_cad_assembly_file()

    log.info('Wrote back to CADAssembly.xml')

if __name__ == '__main__':
    update_parameters()
