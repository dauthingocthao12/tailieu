{extends file="main.tpl"}
{block name=body}

<div class="col-sm-8 col-sm-offset-2">
	<div class="alert alert-success">{$data.message}</div>
	<div class="text-center">
		{makelink mode="site" action="listing" value="一覧へ戻る" class="btn btn-warning"}
	</div>
</div>
{/block}
