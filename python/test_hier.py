from gnuradio import gr, blocks
import ldpc

class my_tb(gr.top_block):
    def __init__(self, fname, sigma, max_iterations):
        gr.top_block.__init__(self)
        
        encoder = ldpc.ldpc_hier_encoder_bf(fname)
        decoder = ldpc.ldpc_hier_decoder_fb(fname,
                sigma, max_iterations)

        inFile = "/home/manu/Downloads/in.flac"
        outFile = "/home/manu/Downloads/out.flac"
        source = blocks.file_source(1, inFile, False)
        sink = blocks.file_sink(1, outFile)

        self.connect(source, encoder, decoder, sink)

def main():
    fname = "/home/manu/repos/ldpc/gr-ldpc/python/alist-files/96.3.963"
    sigma = 0.3
    max_iterations = 100
    tb = my_tb(fname, sigma, max_iterations)
    print "tb initialized"
    tb.start()
    tb.wait()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
