{extends file="main.tpl"}
{block name="main"}

	<h2>Message posted to Facebook</h2>

	{if $success}
		<div class="alert alert-success">Success</div>
	{else}
		<div class="alert alert-danger">Error</div>
	{/if}

{/block}
