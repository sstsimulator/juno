# -*- coding: utf-8 -*-

from sst_unittest import *
from sst_unittest_support import *

import os

################################################################################
# Code to support a single instance module initialize, must be called setUp method

module_init = 0
module_sema = threading.Semaphore()

def initializeTestModule_SingleInstance(class_inst):
    global module_init
    global module_sema

    module_sema.acquire()
    if module_init != 1:
        # Put your single instance Init Code Here
        class_inst._buildJunoAssembler()
        module_init = 1
    module_sema.release()

################################################################################

class testcase_juno(SSTTestCase):

    def initializeClass(self, testName):
        super(type(self), self).initializeClass(testName)
        # Put test based setup code here. it is called before testing starts
        # NOTE: This method is called once for every test

    def setUp(self):
        super(type(self), self).setUp()
        initializeTestModule_SingleInstance(self)
        # Put test based setup code here. it is called once before every test

    def tearDown(self):
        # Put test based teardown code here. it is called once after every test
        super(type(self), self).tearDown()

#####

    def test_juno_sum(self):
        self.juno_test_template("sum")

    def test_juno_modulo(self):
        self.juno_test_template("modulo")

#####

    def juno_test_template(self, testcase):
        # Get the path to the test files
        test_path = self.get_testsuite_dir()
        outdir = self.get_test_output_run_dir()
        tmpdir = self.get_test_output_tmp_dir()
        juno_assembler_dir = "{0}/../asm/".format(test_path)
        juno_assembler = "{0}/sst-juno-asm".format(juno_assembler_dir)
        juno_test_asm_dir = "{0}/../test/asm/".format(test_path)

        # Set the various file paths
        testDataFileName="juno_{0}".format(testcase)

        sdlfile = "{0}/../test/sst/juno-test.py".format(test_path, testcase)
        reffile = "{0}/refFiles/{1}.out".format(test_path, testDataFileName)
        outfile = "{0}/{1}.out".format(outdir, testDataFileName)
        errfile = "{0}/{1}.err".format(outdir, testDataFileName)
        mpioutfiles = "{0}/{1}.testfile".format(outdir, testDataFileName)

        # Assemble the juno program
        test_asm_in_file = "{0}/{1}.juno".format(juno_test_asm_dir, testcase)
        test_bin_out_file = "{0}/juno_{1}.bin".format(tmpdir, testcase)
        test_asm_log_file = "{0}/juno_{1}.log".format(tmpdir, testcase)
        cmd = "{0} -i {1} -o {2} >> {3}".format(juno_assembler, test_asm_in_file, test_bin_out_file, test_asm_log_file)
        cmd_rtn = os.system(cmd)
        asm_success = cmd_rtn == 0
        self.assertTrue(asm_success, "Juno Assembler failed to assemble file {0} into {1}; Return Code = {2}".format(test_asm_in_file, test_bin_out_file, cmd_rtn))

        os.environ["JUNO_EXE"] = test_bin_out_file
        log_debug("JUNO_EXE ENV VAR = {0}".format(os.environ["JUNO_EXE"]))

        self.run_sst(sdlfile, outfile, errfile, mpi_out_files=mpioutfiles)

        # NOTE: THE PASS / FAIL EVALUATIONS ARE PORTED FROM THE SQE BAMBOO
        #       BASED testSuite_XXX.sh THESE SHOULD BE RE-EVALUATED BY THE
        #       DEVELOPER AGAINST THE LATEST VERSION OF SST TO SEE IF THE
        #       TESTS & RESULT FILES ARE STILL VALID

        # Perform the tests
        self.assertFalse(os_test_file(errfile, "-s"), "juno test {0} has Non-empty Error File {1}".format(testDataFileName, errfile))

        # Only check for Simulation is complete in the out and ref file and make sure they match
        out_cmd = 'grep "Simulation is complete" {0}'.format(outfile)
        ref_cmd = 'grep "Simulation is complete" {0}'.format(reffile)

        out_cmd_rtn = os_simple_command(out_cmd)
        ref_cmd_rtn = os_simple_command(ref_cmd)
        log_debug("out_cmd_rtn = {0}\n".format(out_cmd_rtn))
        log_debug("ref_cmd_rtn = {0}\n".format(ref_cmd_rtn))
        cmd_result = out_cmd_rtn[1] == ref_cmd_rtn[1]

        self.assertTrue(cmd_result, "Juno Test; Output found line {0} does not match Ref found line {1}".format(out_cmd_rtn[1], ref_cmd_rtn[1]))

####

    def _buildJunoAssembler(self):
        # Get the path to the test files
        test_path = self.get_testsuite_dir()
        juno_assembler_dir = "{0}/../asm/".format(test_path)

        # Build the juno assembler
        cmd = "make -C {0} > /dev/null".format(juno_assembler_dir)
        cmd_rtn = os.system(cmd)
        build_success = cmd_rtn == 0
        self.assertTrue(build_success, "Juno Assembler failed to build properly; Makefile rtn = {0}".format(cmd_rtn))
