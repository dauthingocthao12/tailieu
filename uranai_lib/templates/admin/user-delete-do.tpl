{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}

{if $data.status=='ERR'}
<div class="alert alert-danger">
	{$data.message}
</div>
{/if}

{if $data.status=='OK'}
<div class="alert alert-success">
	{$data.message}
</div>
{/if}

<div class="text-center">
	{makeLink mode="user" action="listing" value="一覧へ戻る" class="btn btn-primary"}
</div>

{/block}
