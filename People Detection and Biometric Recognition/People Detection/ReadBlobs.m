function [Blobs Scores]=ReadBlobs(filename,video_name,processing_mask,min_score,max_score,threshold)

if nargin < 6
  threshold=-inf;
end


if nargin < 3
    processing_mask.x=0;
    processing_mask.y=0;
    processing_mask.w=inf;
    processing_mask.h=inf;
end

fid=fopen(filename);
cadena=fgets(fid);
num_frame=1;
Blobs={};
Scores=[];
Blobs{num_frame}=[];
flag=0;
while( size(cadena,2)>4 )
    inicios=find(cadena=='(');
    separadores=find(cadena=='/');
    puntos=find(cadena==':');
    num_blobs=size(inicios,2);
   
    if(num_blobs>0)
        aux=sprintf('/%s-%%d.png":',video_name);
        num_frame1=sscanf(cadena(1,separadores(end):puntos(1)),aux);
        aux=sprintf('/frame%%d.png":',video_name);
        num_frame2=sscanf(cadena(1,separadores(end):puntos(1)),aux);
        if isempty(num_frame1)
            num_frame=num_frame2;
        else
            num_frame=num_frame1;
        end
        if(num_frame==0)
            flag=1;
        end
        if(flag)
            num_frame=num_frame+1;
        end

        Temp=sscanf(cadena(1,inicios(1):end),'(%d, %d, %d, %d):%f, ');
        Blobs{num_frame}=[];
        for i=1:num_blobs
            X1=Temp((i-1)*5+1);
            Y1=Temp((i-1)*5+2);
            X2=Temp((i-1)*5+3);
            Y2=Temp((i-1)*5+4);
            Score=Temp((i-1)*5+5);
            if X1<0
                blob.x=0;
            else
                blob.x=X1;
            end
            if Y1<0
                blob.y=0;
            else
                blob.y=Y1;
            end
             blob.w=X2;
             blob.h=Y2;

            blob.num_frame=num_frame;
            blob.score=Score;
            
            if nargin >= 5
                 blob.score=(0.95/(max_score-min_score))*(Score-min_score)+0.05;
            end
           
            
            %%remove blobs outside the process mask
            center.x=blob.x+blob.w/2;
            center.y=blob.y+blob.h/2;

            if(Score>threshold)
                if(center.x>processing_mask.x && center.x<processing_mask.x+processing_mask.w)
                    if(center.y>processing_mask.y && center.y<processing_mask.y+processing_mask.h)
                        Blobs{num_frame}=[Blobs{num_frame} blob];
                        Scores=[Scores blob.score];
                    end
                end
            end
        end
    end
    
    
    cadena=fgets(fid);
    
end

fclose(fid);


