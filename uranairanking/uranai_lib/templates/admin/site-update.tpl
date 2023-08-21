{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
{* TODO: SITE-UPDATE *}

{if $data.status=='OK'}
<div class="alert alert-success">
	{$data.message}
</div>
{else}
<div class="alert alert-danger">
	{$data.message}
</div>
{/if}

{/block}
