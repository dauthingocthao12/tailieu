{extends file="main.tpl"}
{block name="main"}

	<h2>Random Message from aggregated data:</h2>

	<form action="?action=post-facebook-go" method="post">
		<pre>{$facebookmsg}</pre>
		<input type="hidden" name="facebookmsg" value="{$facebookmsg}" />
		<!-- <button type="submit" class="btn btn-primary">Post</button> -->
	</form>

{/block}
