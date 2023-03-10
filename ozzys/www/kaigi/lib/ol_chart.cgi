

# ↓カラーチャート表示
#-----------------------------------------------------------------------------#
package appspage::treecrsdx::main::color_chart;


export appspage::treecrsdx::oo:: 'main';


# →カラーチャート
sub out_chart {
	my $this = shift;
	$this->{'header'}->reset_header;
	$this->{'header'}->set_header('Content-Type: image/png');
	$this->{'header'}->send_header;
	binmode(STDOUT);
	if(!open(CHART,'<'.$this->{'LIB_DIR'}.'chart.png'))
	{ $this->stop; }
	binmode(CHART);
	my $data;
	while(read(CHART,$data,64)) { print STDOUT $data; }
	close(CHART);
	$this->stop;
}




1;
