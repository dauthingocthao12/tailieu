{extends file="main.tpl"}
{block name=body}

<div class="text-right">
	{makelink mode="site" action="detail" id="{$data.site_id}" value="詳細に戻る" class="btn btn-warning"}
</div>
<div class="clear">&nbsp;</div>

<div class="col-sm-8 col-sm-offset-2">

	<form action="" class="well" method="post">
		<div class="site-title">
			サイトID:{$data.site_id}　　サイト名：{$data.site_name}
		</div>
		<div class="form-group">
			<label>サイトdescription</label>
			<textarea name="description" class="form-control" rows="10">{$data.description}</textarea>
		</div>
		
		<div class="form-group">
			<label>本文</label>
			<textarea name="presentation" class="form-control" rows="10">{$data.presentation}</textarea>
		</div>

		<div class="row">
			<div class="col-sm-6">
			<label>表示</label>
				<div class="form-group">
					<input id="visible_yes" name="visible" class="form-radio" type="radio" value="1" {if $data.visible==1}checked{/if}>
					<label for="visible_yes">表示する</label>
				</div>
				<div class="form-group">
					<input id="visible_no" name="visible" class="form-radio" type="radio" value="0" {if $data.visible==0}checked{/if}>
					<label for="visible_no">表示しない</label>
				</div>
			</div>
		</div>

		<hr>

		<dl class="disable">
			<dt>更新日</dt>
			<dd>{$data.date_update|default:"--"}</dd>
		</dl>

		<hr>

		<div class="text-center">
			<input type="hidden" name="mode" value="site">
			<input type="hidden" name="action" value="details_update">
			<input type="hidden" name="id" value="{$data.site_id}">
			<button class="btn btn-success">保存</button>
		</div>
	</form>
</div>

{/block}
