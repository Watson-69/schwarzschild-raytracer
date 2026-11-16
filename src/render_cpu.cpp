// render_cpu.cpp — CPU ray tracer: writes output.ppm (no OpenGL/GLFW needed).
#include "config.h"
#include "geodesic.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace bh;

static double hash3(double x, double y, double z) {
    double p = std::sin(x*12.9898 + y*78.233 + z*37.719) * 43758.5453;
    return p - std::floor(p);
}

static Vec3 starfield(const Vec3& d) {
    double th = std::acos(std::max(-1.0, std::min(1.0, d.y)));
    double ph = std::atan2(d.z, d.x);
    double u = ph/(2*PI)+0.5, v = th/PI;
    Vec3 color{0,0,0};
    for (int i=0;i<3;i++){
        double scale = (i==0?STAR_SCALE_0:(i==1?STAR_SCALE_1:STAR_SCALE_2));
        double gx = std::floor(u*scale), gy = std::floor(v*scale);
        double h = hash3(gx, gy, (double)i);
        if (h > STAR_THRESHOLD) {
            double b = (h-STAR_THRESHOLD)*142.857;
            color.x += b*0.7; color.y += b*0.85; color.z += b;
        }
    }
    double band = std::exp(-std::pow((th-PI*0.5)*3.5,2.0))*0.12;
    color.x += 0.35*band; color.y += 0.45*band; color.z += 0.75*band;
    return color;
}

static Vec3 diskColor(double r, double g) {
    double t = std::pow(DISK_INNER / r, DISK_TEMP_EXPONENT);
    double cool[3]={0.85,0.25,0.05}, hot[3]={1.0,0.92,0.72};
    Vec3 base{ cool[0]+(hot[0]-cool[0])*std::max(0.0,std::min(1.0,t)),
               cool[1]+(hot[1]-cool[1])*std::max(0.0,std::min(1.0,t)),
               cool[2]+(hot[2]-cool[2])*std::max(0.0,std::min(1.0,t)) };
    double intensity = std::pow(std::max(g,0.0),3.0);
    if (intensity>6.0) intensity=6.0;
    base.x *= (0.25+0.75*intensity);
    base.y *= (0.25+0.75*intensity);
    base.z *= (0.25+0.75*intensity);
    return base;
}

int main() {
    int W = 320, H = 180;
    double camR = CAMERA_DISTANCE, camTheta = 1.2, camPhi = 0.0;
    double fov = CAMERA_FOV * PI / 180.0;
    double aspect = (double)W/H;
    double tanHalf = std::tan(fov*0.5);

    Vec3 e_r{std::sin(camTheta)*std::cos(camPhi), std::cos(camTheta), std::sin(camTheta)*std::sin(camPhi)};
    Vec3 e_th{std::cos(camTheta)*std::cos(camPhi), -std::sin(camTheta), std::cos(camTheta)*std::sin(camPhi)};
    Vec3 e_ph{-std::sin(camPhi), 0.0, std::cos(camPhi)};
    Vec3 fwd = e_r * -1.0;
    Vec3 right = e_ph;
    Vec3 up = e_th;

    std::ofstream out("output.ppm");
    out << "P3\n" << W << " " << H << "\n255\n";

    for (int py=0; py<H; py++) {
        for (int px=0; px<W; px++) {
            double u = (2.0*px/(W-1)-1.0)*aspect;
            double v = (1.0 - 2.0*py/(H-1));
            Vec3 dirWorld = normalize(fwd + right*u*tanHalf + up*v*tanHalf);
            double nr = dot(dirWorld, e_r);
            double nth = dot(dirWorld, e_th);
            double nph = dot(dirWorld, e_ph);
            Vec3 dir{nr, nth, nph};

            TraceResult res = traceRay(camR, camTheta, camPhi, dir);

            Vec3 color{0,0,0};
            if (res.kind == TraceResult::Kind::HitHorizon) {
                color = Vec3{0,0,0};
            } else if (res.kind == TraceResult::Kind::Escaped) {
                color = starfield(res.finalDir);
            } else if (res.kind == TraceResult::Kind::HitDisk) {
                color = diskColor(res.diskRadius, res.redshift);
            }
            int r = (int)(std::min(1.0,color.x)*255.0);
            int g = (int)(std::min(1.0,color.y)*255.0);
            int b = (int)(std::min(1.0,color.z)*255.0);
            out << r << " " << g << " " << b << "\n";
        }
    }
    out.close();
    std::cout << "Wrote output.ppm\n";
    return 0;
}