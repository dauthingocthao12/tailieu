{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}

{if $data.status=='OK'}
	<div class="alert alert-success">{$data.message}</div>
{/if}

{if $data.status=='ERR'}
	<div class="alert alert-danger">{$data.message}</div>
{/if}

<hr>

<div class="text-center">
	{makelink mode="news" action="listing" class="btn btn-primary" value="一覧へ戻る"}
</div>

{/block}
