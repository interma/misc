\timing
select gptext.version();

SELECT * FROM gptext.create_index('public', 'wiki', 'id', 'body');

SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*0 and id<=11535325*1), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*1 and id<=11535325*2), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*2 and id<=11535325*3), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*3 and id<=11535325*4), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*4 and id<=11535325*5), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*5 and id<=11535325*6), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*6 and id<=11535325*7), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*7 and id<=11535325*8), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*8 and id<=11535325*9), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*9 and id<=11535325*10), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*10 and id<=11535325*11), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*11 and id<=11535325*12), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*12 and id<=11535325*13), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*13 and id<=11535325*14), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*14 and id<=11535325*15), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*15 and id<=11535325*16), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*16 and id<=11535325*17), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*17 and id<=11535325*18), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*18 and id<=11535325*19), 'demo.public.wiki');
SELECT * FROM gptext.index(TABLE(SELECT * FROM wiki where id>11535325*19 and id<=11535325*20), 'demo.public.wiki');

select * from gptext.commit_index('demo.public.wiki');
