{literal}
<script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');
{/literal}

var qcode = window.location.href
{if $config.is_server}
  ga('create', 'UA-573797-12', 'auto');
  {if $config.AppOS}
	  ga('set','dimension2', "{$config.AppOS} v.{$config.AppVersion}");
  {/if}
  if(qcode = qcode.match(/https:\/\/uranairanking.jp\/\?(.*)/)){
	  ga('set','dimension1', qcode[1]);
	}
  ga('send', 'pageview');
{else}
  ga('create', 'UA-573797-15', 'auto');
  {if $config.AppOS}
	  ga('set','dimension2', "{$config.AppOS} v.{$config.AppVersion}");
  {/if}
  if(qcode = qcode.match(/http:\/\/dev.uranairanking.jp\/\?(.*)/)){
	  ga('set','dimension1', qcode[1]);
	}
	ga('send', 'pageview');
{/if}
</script>
