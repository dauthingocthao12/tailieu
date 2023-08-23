{extends file="main.tpl"}
{block name=body}
<div class="well">
	Logファイル選択：
	{foreach $data.log_files as $file}
		{if $file==$data.log_current}
			{assign "class" "btn-success"}
		{else}
			{assign "class" "btn-primary"}
		{/if}
		{makelink mode="log"
			action="listing"
			file="$file"
			value="$file"
			class="btn btn-xs $class"}
	{/foreach}
	&nbsp;絞込：&nbsp;
	{makelink mode="log" action="listing"
		file=$data.log_current
		filter="ERR"
		value="エラー"
		class="btn btn-xs btn-danger"}
	&nbsp;
	{makelink mode="log" action="listing"
		file=$data.log_current
		filter="OK"
		value="サクセス"
		class="btn btn-xs btn-success"}
</div>

<table id="log-table" class="table table-bordered table-striped table-condensed">
	<thead>
		<tr>
			<th>日付・時間 🔻</th>
			<th>タイトル</th>
			<th>メッセージ</th>
		</tr>
	</thead>
	{foreach $data.log_lines as $line}
	<tr class="{$line.class}">
		{if $line[1]==='STOP'}
			{cycle name="group" values="batch-group1,batch-group2" assign="group"}
		{/if}
		<td class="nowrap {$group|default:'batch-group1'}">
			{$line[0]}
		</td>
		<td class="nowrap">
			{$line[1]}
		</td>
		<td>
			{$line[2]}
		</td>
	</tr>
	{/foreach}
</table>
{/block}
