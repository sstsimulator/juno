import os
import sst

# Define SST core options
sst.setProgramOption("timebase", "1ps")

# Tell SST what statistics handling we want
sst.setStatisticLoadLevel(4)

# Define the simulation components
comp_cpu = sst.Component("cpu", "juno.JunoCPU")
comp_cpu.addParams({
	"verbose" : 0,
	"registers" : 16,
	"program" : os.getenv("JUNO_EXE", "../asm/gups.bin"),
	"clock" : "2.4GHz",
	"cycles-add" : 1,
	"max-address" : 16 * 1024 * 1024 * 1024
})

# Define external RAND accelerator
rand_accel = sst.Component("randacc", "juno.JunoRandAccelerator")
rand_accel.addParams({
	"verbose" : 0
})

# Define RAND support
randsc = comp_cpu.setSubComponent("customhandler", "juno.JunoExternalRandomHandler")

cpu_rand_link = sst.Link("cpu_rand_accel_link")
cpu_rand_link.connect(  (randsc, "genlink", "2ns"),
			(rand_accel, "cpulink", "2ns") )

comp_l1cache = sst.Component("l1cache", "memHierarchy.Cache")
comp_l1cache.addParams({
      "access_latency_cycles" : "2",
      "cache_frequency" : "2.4 GHz",
      "replacement_policy" : "lru",
      "coherence_protocol" : "MESI",
      "associativity" : "4",
      "cache_line_size" : "64",
      "prefetcher" : "cassini.StridePrefetcher",
      "debug" : "1",
      "L1" : "1",
      "cache_size" : "512B"
})

comp_memory = sst.Component("memory", "memHierarchy.MemController")
comp_memory.addParams({
      "coherence_protocol" : "MESI",
      "backend.access_time" : "10 ns",
      "backend.mem_size" : "16GiB",
      "clock" : "1GHz"
})

sst.setStatisticOutput("sst.statOutputCSV")
sst.enableAllStatisticsForAllComponents()

sst.setStatisticOutputOptions( {
        "filepath"  : "output.csv"
} )

# Define the simulation links
link_cpu_cache_link = sst.Link("link_cpu_cache_link")
link_cpu_cache_link.connect( (comp_cpu, "cache_link", "1000ps"), (comp_l1cache, "high_network_0", "1000ps") )
link_cpu_cache_link.setNoCut()

link_mem_bus_link = sst.Link("link_mem_bus_link")
link_mem_bus_link.connect( (comp_l1cache, "low_network_0", "50ps"), (comp_memory, "direct_link", "50ps") )
