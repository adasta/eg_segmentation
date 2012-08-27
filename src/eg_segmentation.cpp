
#include <eg_segmentation/eg_segmentation.h>

#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "filter.h"
#include "segment-graph.h"


template<typename T>
float diff( const T& a, const T& b){
  return fabs(a-b);
}

template<>
float diff <cv::Vec3b>( const cv::Vec3b& a, const cv::Vec3b& b){
  float v=0;
  for(int i=0; i<3; i++) v+=pow(a[i]-b[i],2);
  return sqrt(v);
}


template<typename T>
edge * build_edge_graph( const cv::Mat& img, int& num_edges){
  // build graph
     edge *edges = new edge[img.rows*img.cols*4];
      num_edges = 0;
     for (int y = 0; y < img.rows; y++) {
       const  T* above,* below, *current;
       current = img.ptr<T>(y) ;
       if (y>0) below = img.ptr<T>(y-1);
       if (y< img.rows) above = img.ptr<T>(y+1);

       for (int x = 0; x < img.cols; x++) {
         if (x < img.cols-1) {
           edges[num_edges].a = y * img.cols + x;
           edges[num_edges].b = y * img.cols + (x+1);
           edges[num_edges].w = diff(current[x], current[x+1]);
           num_edges++;
         }

         if (y < img.rows-1) {
     edges[num_edges].a = y * img.cols + x;
     edges[num_edges].b = (y+1) * img.cols + x;
     edges[num_edges].w = diff(current[x], above[x]);
     num_edges++;
         }

         if ((x < img.cols-1) && (y < img.rows-1)) {
     edges[num_edges].a = y * img.cols + x;
     edges[num_edges].b = (y+1) * img.cols + (x+1);
     edges[num_edges].w = diff(current[x],  above[x+1]);
     num_edges++;
         }

         if ((x < img.cols-1) && (y > 0)) {
     edges[num_edges].a = y * img.cols + x;
     edges[num_edges].b = (y-1) * img.cols + (x+1);
     edges[num_edges].w = diff(current[x], below[x+1] );
     num_edges++;
         }
       }
     }
     return edges;
}


int cv::segment_image (const cv::Mat& input, cv::Mat& output, float sigma,
    int k, int min_size)
{

  int width = input.cols ;
  int height = input.rows ;

  if(sigma > 0.001){
    cv::GaussianBlur(input,output, cv::Size(1.5*sigma,1.5*sigma), sigma, sigma);
  }
  else{
    output = input.clone();
  }

  edge* edges;

  int num;
  if (input.type() == CV_8UC3){
    edges = build_edge_graph<cv::Vec3b>( output, num);
  }
  if (input.type() == CV_8UC1){
    edges = build_edge_graph<uint8_t>( output, num);
  }
  if (input.type() == CV_32F){
      edges = build_edge_graph<float>( output, num);
    }
  else{
    std::cerr <<"[EG_SEGMENTATION] UNSUPPORTED INPUT TYPE\n";
  }
    // segment
    universe *u = segment_graph(width*height, num, edges, k);

    // post process small components
    for (int i = 0; i < num; i++) {
      int a = u->find(edges[i].a);
      int b = u->find(edges[i].b);
      if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
        u->join(a, b);
    }
    std::map<int, int> remap;
    int id_counter=0;
    output.create(height, width, CV_16UC1);
    for (int y = 0; y < height; y++) {
      uint16_t* orow = output.ptr<uint16_t>(y);
       for (int x = 0; x < width; x++) {
         int comp = u->find(y * width + x);
         std::map<int,int>::iterator id_iter;
         id_iter = remap.find(comp);
         if (id_iter == remap.end()){
          remap[comp] =id_counter;
          id_counter++;
          id_iter = remap.find(comp);
         }
         orow[x] = id_iter->second;
       }
     }

    delete [] edges;
    int num_ccs = u->num_sets();
    delete u;

    return num_ccs;
}

void cv::colorize_index_image (const cv::Mat& input, cv::Mat& output,
      ColorMap& map)
{
  output.create(input.rows, input.cols, CV_8UC3);

}
